#! /bin/bash
# 判断需要编译类型
echo "判断需要编译类型为$1"
echo "ip is $2"
if [ "$1" == "lib" ];then
# 编译lib
	echo "准备编译lib!"
	find Makefile_so
	if [ $? -eq 0 ];then
		mv Makefile_so Makefile
		if [ $? -eq 0 ];then
			echo "lib库Makefile部署成功"
			make clean
			make
			if [ $? -eq 0 ];then
				echo "so编译成功"
			else
				echo "so编译失败"
			fi
		else
			echo "lib库Makefile部署失败"
		fi
		mv Makefile Makefile_so
		if [ x"$2" != x ];then
			scp -P 22 ./libs/* root@$2:/wanji/v2x_bin/lib/
			if [ $? -eq 0 ];then
				echo "lib库下载到设备成功"
			else
				echo "lib库下载到设备失败"
			fi
		else
			echo "no upload"
		fi
	else
		echo "请确认存在Makefile_so"
	fi
elif [ "$1" == "app" ];then
# 编译可执行文件
	echo "准备编译可执行文件"
	find Makefile_app
	if [ $? -eq 0 ];then
		mv Makefile_app Makefile
		if [ $? -eq 0 ];then
			echo "可执行Makefile部署成功"
			make clean
			make
			if [ $? -eq 0 ];then
				echo "可执行文件编译成功"
			else
				echo "可执行文件编译失败"
			fi
		else
			echo "可执行Makefile部署失败"
		fi
		mv Makefile Makefile_app
		if [ x"$2" != x ];then
			scp -P 22 ./V2X_APP_LTEV root@$2:/wanji/v2x_bin/bin/V2X_APP_LTEV
			if [ $? -eq 0 ];then
				echo "可执行文件下载到设备成功"
			else
				echo "可执行文件下载到设备失败"
			fi
		else
			echo "no upload"
		fi
	else
		echo "请确认存在Makefile_app"
	fi
elif [ "$1" == "all" ];then
# 编译所有
	find Makefile_app Makefile_so
	if [ $? -eq 0 ];then
		mv Makefile_so Makefile
		if [ $? -eq 0 ];then
			echo "lib库Makefile部署成功"
			make clean
			make
			if [ $? -eq 0 ];then
				echo "so编译成功"
				mv Makefile Makefile_so
				mv Makefile_app Makefile
				if [ $? -eq 0 ];then
					echo "可执行Makefile部署成功"
					make clean
					make
					if [ $? -eq 0 ];then
						echo "可执行文件编译成功"
						mv Makefile Makefile_app
						if [ x"$2" != x ];then
							scp -P 22 ./V2X_APP_LTEV root@$2:/wanji/v2x_bin/bin/V2X_APP_LTEV
							if [ $? -eq 0 ];then
								echo "可执行文件下载到设备成功"
							else
								echo "可执行文件下载到设备失败"
							fi
						else
							echo "no upload" 
						fi
						if [ x"$2" != x ];then
							scp -P 22 ./libs/* root@$2:/wanji/v2x_bin/lib/
							if [ $? -eq 0 ];then
								echo "lib库下载到设备成功"
							else
								echo "lib库下载到设备失败"
							fi
						else
							echo "no upload"
						fi
					else
						echo "可执行文件编译失败"
						mv Makefile Makefile_app
					fi
				else
					echo "可执行Makefile部署失败"
					mv Makefile Makefile_app
				fi
			else
				echo "so编译失败"
				mv Makefile Makefile_so
			fi
		else
			echo "lib库Makefile部署失败"
		fi
	else
		echo "请确认存在Makefile_app 和Makefile_so"
	fi
else
# 输入错误
	echo "input is err"
fi

