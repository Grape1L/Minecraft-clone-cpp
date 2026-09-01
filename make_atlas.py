from PIL import Image

side = Image.open("Textures/RozaBlock2/side.jpg")
top = Image.open("Textures/RozaBlock2/top.jpg")
bottom = Image.open("Textures/RozaBlock2/bottom.jpg")

tile_size = side.size[0]
atlas = Image.new("RGBA", (tile_size * 3, tile_size))

atlas.paste(side, (0 * tile_size, 0))
atlas.paste(top, (1 * tile_size, 0))
atlas.paste(bottom, (2 * tile_size, 0))

atlas.save("Textures/RozaBlock2/roza_block_atlas.png")