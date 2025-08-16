# this is also chatgpt generated
# sorry
# but holy shit can you believe it wrote all this??

#!/usr/bin/env python3
import sys
import math
import argparse
from PIL import Image, ImageDraw, ImageFont

CHARS = [chr(c) for c in range(ord('A'), ord('Z') + 1)]
IMG_W = 128
IMG_H = 128
THRESHOLD = 128  # grayscale threshold to 1-bit
BITS_PER_WORD = 64

def load_font(path: str | None, target_px: int) -> ImageFont.FreeTypeFont | ImageFont.ImageFont:
    """
    Load a font. Prefer a TrueType at roughly target_px, else fall back to DejaVuSans,
    else Pillow's default (bitmap). We'll size/center later.
    """
    if path:
        try:
            return ImageFont.truetype(path, size=target_px)
        except Exception as e:
            print(f"Warning: failed to load '{path}': {e}", file=sys.stderr)
    # Try a common bundled font
    for name in ("DejaVuSans.ttf", "DejaVuSansMono.ttf"):
        try:
            return ImageFont.truetype(name, size=target_px)
        except Exception:
            pass
    # Last resort
    return ImageFont.load_default()

def fit_font_to_box(font_path: str | None, box_w: int, box_h: int, char: str, margin: int = 4):
    """
    Find a font size that fits 'char' within (box_w, box_h) minus margins.
    Binary searches a TTF size if possible; if using bitmap default, just return it.
    """
    # If we didn't get a FreeType font, just return it as-is
    try:
        # Probe if truetype by checking getname()/path via attribute
        _ = ImageFont.truetype  # noqa
        truetype_available = True
    except Exception:
        truetype_available = False

    # If we have a real TTF, binary search size
    lo, hi = 8, max(box_w, box_h) * 2
    best_font = load_font(font_path, lo)
    best_bbox_area = 0

    # If best_font is bitmap default, it won't resize; just center it and go.
    is_bitmap_default = not hasattr(best_font, "path")  # crude but works
    if is_bitmap_default:
        return best_font

    # Binary search on size
    while lo <= hi:
        mid = (lo + hi) // 2
        f = load_font(font_path, mid)
        # Use getbbox for accurate extents
        bbox = f.getbbox(char)
        w = bbox[2] - bbox[0]
        h = bbox[3] - bbox[1]
        if w <= (box_w - 2 * margin) and h <= (box_h - 2 * margin):
            best_font = f
            best_bbox_area = w * h
            lo = mid + 1
        else:
            hi = mid - 1

    return best_font

def render_char_to_image(font, ch: str, w: int, h: int) -> Image.Image:
    """
    Render a single character centered into a grayscale image, then threshold to 1-bit.
    White glyph (1) on black background (0).
    """
    img = Image.new("L", (w, h), 0)
    draw = ImageDraw.Draw(img)

    # Measure
    bbox = font.getbbox(ch)
    gw = bbox[2] - bbox[0]
    gh = bbox[3] - bbox[1]

    # Centering: baseline offset matters, so use bbox offset
    x = (w - gw) // 2 - bbox[0]
    y = (h - gh) // 2 - bbox[1]

    # Draw white text
    draw.text((x, y), ch, fill=255, font=font)

    # Threshold to 1-bit
    img1 = img.point(lambda p: 255 if p >= THRESHOLD else 0, mode="1")
    return img1

def pack_image_to_u64(img: Image.Image) -> list[int]:
    """
    Pack a 1-bit image into uint64_t words, row-major, left->right, top->bottom,
    with the leftmost pixel in the LSB of the word.
    """
    if img.mode != "1":
        img = img.convert("1")
    w, h = img.size
    data = list(img.getdata())  # each is 0 or 255
    words = []
    word = 0
    bitpos = 0
    for p in data:
        bit = 1 if p else 0
        if bit:
            word |= (1 << bitpos)
        bitpos += 1
        if bitpos == BITS_PER_WORD:
            words.append(word)
            word = 0
            bitpos = 0
    if bitpos > 0:
        words.append(word)
    return words

def emit_c_header(words_by_char: dict[str, list[int]], width: int, height: int):
    """
    Print a C-ish blob to stdout:
      - includes stdint.h
      - per-glyph packed arrays `glyph_<ch>[]`
      - a table with metadata mapping 'A'..'Z' to arrays
    """
    print("// Auto-generated glyph pack")
    print("//  - 1 bit per pixel, LSB = leftmost pixel")
    print("//  - row-major: left->right, top->bottom")
    print(f"//  - glyph size: {width}x{height}")
    print("#include <stdint.h>")
    print("#include <stddef.h>")
    print()

    # Emit arrays
    for ch, words in words_by_char.items():
        print(f"static const uint64_t glyph_{ch}[] = {{")
        # nice formatting: 4 words per line
        for i, w64 in enumerate(words):
            end = "\n" if (i + 1) % 4 == 0 else " "
            print(f"    0x{w64:016x}ULL,", end=end)
        if len(words) % 4 != 0:
            print()
        print("};")
        print()

    # Emit metadata struct and table
    print("typedef struct {")
    print("    char ch;")
    print("    uint32_t width;")
    print("    uint32_t height;")
    print("    const uint64_t *data;")
    print("    size_t data_len;")
    print("} Glyph;")
    print()

    print(f"static const Glyph FONT_A_Z[{len(words_by_char)}] = {{")
    for ch, words in words_by_char.items():
        print(f"    {{ '{ch}', {width}, {height}, glyph_{ch}, sizeof(glyph_{ch})/sizeof(uint64_t) }},")
    print("};")

def main():
    ap = argparse.ArgumentParser(description="Render A–Z to 128x128 PNGs and emit packed C arrays.")
    ap.add_argument("--font", type=str, default=None, help="Path to a .ttf/.otf font to use")
    ap.add_argument("--outdir", type=str, default=".", help="Directory to write PNGs")
    ap.add_argument("--invert", action="store_true", help="Invert: 1=background, 0=glyph")
    args = ap.parse_args()

    words_by_char: dict[str, list[int]] = {}

    for ch in CHARS:
        font = fit_font_to_box(args.font, IMG_W, IMG_H, ch)
        img1 = render_char_to_image(font, ch, IMG_W, IMG_H)

        # Optional invert (if you want 1 = background, 0 = glyph)
        if args.invert:
            img1 = Image.eval(img1, lambda p: 0 if p else 255)

        # Save PNG
        png_path = f"{args.outdir}/{ch}.png"
        img1.save(png_path)

        # Pack to u64
        words = pack_image_to_u64(img1)
        words_by_char[ch] = words

    # Dump C to stdout
    emit_c_header(words_by_char, IMG_W, IMG_H)

if __name__ == "__main__":
    main()
