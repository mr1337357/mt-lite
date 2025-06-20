VERSION=v5.4.1
. ~/esp/${VERSION}/esp-idf/export.sh

case $1 in
	config)
		cp main/boards/$2/config .config
		echo configured for $2
		;;
	build)
		idf.py build
		;;
	flash)
		idf.py -p /dev/ttyUSB0 flash
		;;
esac
