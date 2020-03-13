if [ ! -f 'build_config.local' ]; then
	echo "No build_config.local found. Aborting."
	exit 1
fi

source ./'build_config.local'

ndk-build clean && ndk-build