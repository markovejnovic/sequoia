# sequoia

Sequoia is a pipeline and interface for identifying lumber defects and automatic
grading.

## Dependencies

### Building

To build, you need: `meson`, `ninja-build`, `boost`.

### Testing

Testing is very convoluted -- you need `gstreamer`, `v4l2loopback` and `ffmpeg`.

## Testing

First, setup a `v4l2` loopback device:
```bash
sudo modprobe v4l2loopback video_nr=10
```

Then, create a gstreamer stream into the new device:
```bash
sudo gst-launch-1.0 filesrc location="tests/data/5.jpg" ! decodebin ! imagefreeze ! v4l2sink device=/dev/video10
```
