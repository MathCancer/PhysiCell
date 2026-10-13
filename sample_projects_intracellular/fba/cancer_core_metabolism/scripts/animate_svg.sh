#!/usr/bin/env bash

# Usage:
# ./svg_to_video.sh <svg_folder> <output_video.mp4> [fps] [width]

set -e

ROOT_DIR=$(dirname "$(realpath "$0")")


ROOT_DIR="$(dirname "$(realpath "$0")")"

# If argument provided → use it
# Otherwise → default path
SVG_DIR="${ROOT_DIR}/../${1:-output}" # Default to ../output/svg if not provided
OUTPUT_VIDEO="$2"
FPS="${3:-24}"
WIDTH="${4:-2000}"

if [ -z "$SVG_DIR" ] || [ -z "$OUTPUT_VIDEO" ]; then
  echo "Usage: $0 <svg_folder> <output_video.mp4> [fps] [width]"
  exit 1
fi

TMP_DIR=$(mktemp -d)

echo "Temporary frame directory: $TMP_DIR"
echo $SVG_DIR
# Sort SVG files to ensure correct order
FILES=$(ls "$SVG_DIR"/snapshot*.svg | sort)

i=0
for f in $FILES; do
    printf -v num "%05d" $i
    png="$TMP_DIR/frame_${num}.png"

    inkscape "$f" \
        --export-type=png \
        --export-width="$WIDTH" \
        --export-filename="$png" >/dev/null 2>&1

    ((++i))
done

echo "Rendering video..."

ffmpeg -y \
    -framerate "$FPS" \
    -i "$TMP_DIR/frame_%05d.png" \
    -c:v libx264 \
    -crf 18 \
    -pix_fmt yuv420p \
    "$OUTPUT_VIDEO"

echo "Cleaning up..."
rm -rf "$TMP_DIR"

echo "Done! Video saved as $OUTPUT_VIDEO"
