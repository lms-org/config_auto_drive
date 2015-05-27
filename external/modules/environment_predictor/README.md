# image_converter

The image converter module is doing the following in each cycle:
- Read an image from a channel A
- Convert this image into another image format (e.g. from YUYV to GREY)
- Write the converted image to a channel B

## DataChannels
- **INPUT_IMAGE** - The module will read an image from this channel
- **OUTPUT_IMAGE** - The module will write the converted image into this channel.

## Config
- **output_format** - Look into [imaging lib](https://github.com/syxolk/imaging) for supported formats and conversions.

## Dependencies
- [imaging](https://github.com/syxolk/imaging)

## Recommended modules
- [image_loader](https://github.com/syxolk/image_loader) - to convert an image loaded from local hard drive
- [camera_importer](https://github.com/Phibedy/camera) - to convert an image after reading from a camera
- [image_logger](https://github.com/syxolk/image_logger) - to convert an image before saved to local hard drive
