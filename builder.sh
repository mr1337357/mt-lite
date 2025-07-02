VERSION=v5.4.1
. ~/esp/${VERSION}/esp-idf/export.sh > .idflog 2> .idferr

case $1 in
	clean)
		rm .config
		rm -r build/
		rm sdkconfig
		rm sdkconfig.old
		;;
	idf)
		shift
		idf.py $@
		;;
	config)
		if [[ "$2" == "" ]]
		then
			echo "no board specified." 
			echo "boards:"
			ls main/boards
			exit 1
		fi
		cp main/boards/$2/config .config 
		cp main/boards/$2/sdkconfig sdkconfig
		idf.py build-system-targets >> .idflog 2>> .idferr
		cp main/boards/$2/sdkconfig.json build/config/
		echo configured for $2
		;;
	build)
		if [ ! -f .config ]
		then
			echo "not configured! run \"builder.py config\" first"
			exit 1
		fi
		idf.py build
		;;
	flash)
		idf.py -p /dev/ttyUSB0 flash
		;;
	term)
		idf.py monitor
		;;
	*)
		idf.py --help
		;;

esac
