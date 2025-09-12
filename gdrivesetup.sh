rclone mount --daemon --vfs-cache-mode full --vfs-cache-max-size=15G --vfs-cache-max-age=12h --buffer-size 64M --vfs-read-ahead 4G --no-modtime --dir-cache-time 1h Zdrive:/ /$HOME/Documents/Zdrive/
rclone mount --daemon --vfs-cache-mode full --vfs-cache-max-size=15G --vfs-cache-max-age=12h --buffer-size 64M --vfs-read-ahead 4G --no-modtime --dir-cache-time 1h Rdrive:/ /$HOME/Documents/Rdrive/
