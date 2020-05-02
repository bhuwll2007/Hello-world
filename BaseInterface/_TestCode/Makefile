######################################################################################
#
# Master Makefile
#
########################################################################################

# Build groups
BG_BASEINTERFACE	:= 
BG_BASEINTERFACE	+= lib-canutil
BG_BASEINTERFACE	+= lib-confdb
BG_BASEINTERFACE	+= lib-confutil
BG_BASEINTERFACE	+= lib-databaseutil
BG_BASEINTERFACE	+= lib-gpsutil
BG_BASEINTERFACE	+= lib-net_TcpTest
BG_BASEINTERFACE	+= lib-netutil
BG_BASEINTERFACE	+= lib-serialutil
BG_BASEINTERFACE	+= lib-StructFun_Test
BG_BASEINTERFACE	+= lib-Test_Code1
BG_BASEINTERFACE	+= lib-Test_Code2
BG_BASEINTERFACE	+= lib-Test_Sem
BG_BASEINTERFACE	+= lib-V2XUtil_DSRC
BG_BASEINTERFACE	+= lib-V2XUtil_LTEV

BG_ALL          	:=
BG_ALL 				+=$(BG_BASEINTERFACE)

# Build list
BL		?= $(BG_ALL)

all:
	@for target in `echo $(BL) | tr , ' '` ; do  \
		if ! $(MAKE) $${target} ; then  \
			exit 1 ;  \
		fi ;  \
	done
	$(CPYDIR)

clean:
	@rm -f .toolchain
	@sudo rm -rf $(INSTALLDIR)
	@for target in `echo $(BL) | tr , ' '` ; do  \
		$(MAKE) $${target} CMD=clean ; \
	done
	$(DELDIR)

################################################################################
#
# Software Components Build Rule
#
################################################################################

##
## BG_BASEINTERFACE components:
##

lib-canutil:
	@$(MAKE) -C CANUtil $(CMD)

lib-confdb:
	@$(MAKE) -C ConfDB $(CMD)

lib-confutil:
	@$(MAKE) -C ConfUtil $(CMD)

lib-databaseutil:
	@$(MAKE) -C DataBaseUtil $(CMD)

lib-gpsutil:
	@$(MAKE) -C GPSUtil $(CMD)

lib-net_TcpTest:
	@$(MAKE) -C Net_TcpTest $(CMD)
	
lib-netutil:
	@$(MAKE) -C NetUtil $(CMD)

lib-serialutil:
	@$(MAKE) -C SerialUtil $(CMD)
	
lib-StructFun_Test:
	@$(MAKE) -C StructFun_Test $(CMD)
	
lib-Test_Code1:
	@$(MAKE) -C Test_Code1 $(CMD)
	
lib-Test_Code2:
	@$(MAKE) -C Test_Code2 $(CMD)
	
lib-Test_Sem:
	@$(MAKE) -C Test_Sem $(CMD)
	
lib-V2XUtil_DSRC:
	@$(MAKE) -C V2XUtil_DSRC $(CMD)

lib-V2XUtil_LTEV:
	@$(MAKE) -C V2XUtil_LTEV $(CMD)

.CPYDIR:
CPYDIR = 

.DELDIR:
DELDIR =