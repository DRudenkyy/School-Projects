#!/bin/sh

# Change these per your tastes (setting $quiet to "true" will
# suppress move of the output, for example).
container=docker-gl
image=jake-brandt/docker-gl
port=6080
extra_run_args=""
quiet=""

while [ $# -gt 0 ]; do
	case "$1" in
		-h)
			show_help
			exit 0
			;;
		-c)
			container=$2
			shift
			;;
		-i)
			image=$2
			shift
			;;
		-p)
			port=$2
			shift
			;;
		-r)
			extra_run_args="$extra_run_args $2"
			shift
			;;
		-q)
			quiet=1
			;;
		*)
			show_help >&2
			exit 1
			;;
	esac
	shift
done

url="http://localhost:$port"

cleanup() {
	docker stop $container >/dev/null
	docker rm $container >/dev/null
}

# Check to see if an instance is already running; if so, kill it.
running=$(docker ps -a -q --filter "name=${container}")
if [ -n "$running" ]; then
	if [ -z "$quiet" ]; then
		echo "Stopping and removing the previous session..."
		echo ""
	fi
	cleanup
fi

# Display the web host details if not running in quiet mode
if [ -z "$quiet" ]; then
	echo ""
	echo "Setting up the graphical application container..."
	echo ""
	if [ -n "$port" ]; then
		echo "Point your web browser to ${url}"
		echo ""
	fi
fi

# If port is set, expose it (pretty sure the base Docker image might
# be doing this too; but this allows us to override the external port
# to be something other 6080).
port_arg=""
if [ -n "$port" ]; then
	port_arg="-p $port:6080"
fi

docker run \
  -d \
  --name $container \
  $port_arg \
  $extra_run_args \
  $image >/dev/null

print_app_output() {
	docker cp $container:/var/log/supervisor/graphical-app-launcher.log - \
		| tar xO
	result=$(docker cp $container:/tmp/graphical-app.return_code - \
		| tar xO)
	cleanup
	exit $result
}

trap "docker stop $container >/dev/null && print_app_output" SIGINT SIGTERM

docker wait $container >/dev/null

print_app_output

# vim: noexpandtab shiftwidth=4 tabstop=4 softtabstop=0
