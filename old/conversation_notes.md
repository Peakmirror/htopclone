# Conversation Notes

## What we discussed
- You’re learning C by building a small `htop` clone.
- Your CPU section was copied from the memory section, but `/proc/stat` has a different format.
- For CPU usage, the important idea is to parse all CPU time fields, then compute:
  - total = sum of all fields
  - idle = idle + iowait
  - used = total - idle
- You want mentor-style help, not full code unless you ask for it.

## Next step
- Rewrite the CPU parser around the real `/proc/stat` format instead of treating it like `/proc/meminfo`.
