#!env python3
import sys
import io
from PIL import Image, ImageFilter

with io.BytesIO() as output:
    with Image.open(sys.argv[1], 'r')  as im:
        print(im.format, im.size, im.mode, im.info)

        im_first_line = im.crop((0, 0, im.height, 1))
        im_first_line.save("header.tif")
        im_first_line.save(output, format="TIFF")

        print(im_first_line.TiffTags.lookup(0))

#        header_data = output.getvalue()

#        print(header_data)

