# this is chatgpt generated
# sorry :(

from PIL import Image

# Load image as 1-bit
img = Image.open("greetz_crop.png").convert("1")
w, h = img.size
pixels = [1 if p else 0 for p in img.getdata()]

# Pack bits into 64-bit words
words = []
word = 0
bitpos = 0

for p in pixels:
    if p:
        word |= (1 << bitpos)
    bitpos += 1
    if bitpos == 64:
        words.append(word)
        word = 0
        bitpos = 0

# Handle trailing bits
if bitpos > 0:
    words.append(word)

# Print C array
print(f"// Image size: {w}x{h}, total pixels: {w*h}")
print(f"#define GREETZ_WIDTH ({w})")
print(f"#define GREETZ_HEIGHT ({h})")
print(f"const unsigned long long huge IMAGE[{len(words)}] = {{")

for i, w64 in enumerate(words):
    print(f"    0x{w64:016x}ULL,", end="")
    if (i + 1) % 4 == 0:
        print()
    else:
        print(" ", end="")
print("\n};")
