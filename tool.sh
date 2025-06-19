VERSION=v5.4.1
. ~/esp/${VERSION}/esp-idf/export.sh

case $1 in
	build)
		idf.py build
		;;
	flash)
		idf.py -p /dev/ttyUSB0 flash
		;;
esac
