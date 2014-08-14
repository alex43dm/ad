ACLOCAL_AMFLAGS = -I m4

SRC_ALL=\
src/redis_server.cpp \
src/EBranch.cpp \
src/Server.cpp \
src/DB.cpp \
src/KompexSQLiteStatement.cpp \
src/GeoIPTools.cpp \
src/DataBase.cpp \
src/BaseCore.cpp \
src/Config.cpp \
src/Informer.cpp \
src/Log.cpp \
src/CgiService.cpp \
src/ParentDB.cpp \
src/KompexSQLiteDatabase.cpp \
src/Params.cpp \
src/Cookie.cpp \
src/Core.cpp \
src/Offer.cpp \
src/RedisClient.cpp \
src/json.cpp \
src/base64.cpp \
src/Campaign.cpp \
src/UrlParser.cpp \
src/GeoRerions.cpp \
main.cpp


CMN_CPPFLAGS=-std=c++11 -Iinclude
CMN_LDADD = $(BOOST_DATE_TIME_LIBS) $(BOOST_FILESYSTEM_LIBS) $(BOOST_THREAD_LIBS) $(BOOST_SYSTEM_LIBS) $(BOOST_REGEX_LIBS)


SUBDIRS = libs/libredis

if DEBUG
bin_PROGRAMS = rgadtrackerd-dev
rgadtrackerd_dev_SOURCES=$(SRC_ALL)
rgadtrackerd_dev_CPPFLAGS=$(CMN_CPPFLAGS) -Ilibs/libredis
rgadtrackerd_dev_LDADD=$(CMN_LDADD) libs/libredis/lib/libredis.a
else
bin_PROGRAMS = rgadtrackerd
rgadtrackerd_SOURCES=$(SRC_ALL)
rgadtrackerd_CPPFLAGS=$(CMN_CPPFLAGS) -Ilibs/libredis
rgadtrackerd_LDADD=$(CMN_LDADD) libs/libredis/lib/libredis.a
endif

DEBDEP=bash (>= 2.05a-11), awk, procps (>= 1:2.0.7-8), sed (>= 3.02-8), grep (>= 2.4.2-3), coreutils (>= 5.0-5), libtinyxml2.6.2 (>=2.6.2), libsqlite3-0 (>=3.7.13), libfcgi0ldbl (>=2.4.0), sphinxsearch (>=2.1.5), geoip-database (>=20130213-1), rabbitmq-server (>=3.2.3), libboost-system1.49.0 (>=1.49.0), libboost-regex1.49.0 (>=1.49.0), libboost-thread1.49.0 (>=1.49.0), libboost-filesystem1.49.0 (>=1.49.0), libboost-date-time1.49.0 (>=1.49.0)

if DEBUG
AM_CFLAGS = -g -DDEBUG -Wall
AM_CXXFLAGS = -g -DDEBUG -Wall
MAKEINIT=0
else
AM_CFLAGS = -O2
AM_CXXFLAGS = -O2
MAKEINIT=1
endif

debian_BUILD = $(bin_PROGRAMS)-$(VERSION)

dist_doc_DATA=README

sysconf_DATA= \
db_dump/requests/all.sql \
db_dump/tables/11_GeoCountries.sql \
db_dump/tables/00_Campaign.sql \
db_dump/tables/17_Retargeting.sql \
db_dump/tables/12_GeoRerions.sql \
db_dump/tables/20_GeoLiteCity.sql \
db_dump/tables/06_Accounts.sql \
db_dump/tables/02_Offer.sql \
db_dump/tables/07_Campaign2Accounts.sql \
db_dump/tables/15_CampaignNow.sql \
db_dump/tables/16_Informer.sql \
db_dump/tables/18_Informer2OfferRating.sql \
db_dump/tables/10_CronCampaign.sql \
db_dump/tables/05_Campaign2Informer.sql \
db_dump/tables/14_regionTargeting.sql \
db_dump/tables/08_Domains.sql \
db_dump/tables/19_Campaign2Categories.sql \
db_dump/tables/04_Categories2Domain.sql \
db_dump/tables/13_geoTargeting.sql \
db_dump/tables/03_Categories.sql \
db_dump/tables/09_Campaign2Domains.sql \
db_dump/tables/04_Categories2Informer.sql \
db_dump/view/00_Campaign2GeoRerions.sql \
db_dump/view/03_Campaign2Acnts.sql \
db_dump/view/02_Campaign2Doms.sql \
db_dump/view/04_Campaign2Infs.sql \
js/_t.js \
js/_tc.js \
templates/out.html \
templates/error.html

install_f =\
echo -n "install to $1"; \
$(MKDIR_P) $(1)/etc/init.d; \
$(MKDIR_P) $(1)/usr/sbin; \
$(SED) 's|/home/alex/Projects/rgadtrackerd|/etc/$(bin_PROGRAMS)|' < ./config.xml > $(1)/etc/$(bin_PROGRAMS)/config-$(bin_PROGRAMS).xml; \
$(INSTALL) $(bin_PROGRAMS) $(1)/usr/sbin; \
if [ $(MAKEINIT) -eq 1 ]; then \
$(SED) 's|/etc/rgadtrackerd/config.xml|/etc/$(bin_PROGRAMS)/config-$(bin_PROGRAMS).xml|;s|/usr/sbin/rgadtrackerd|/usr/sbin/$(bin_PROGRAMS)|' < ./install/rgadtrackerd > $(1)/etc/init.d/$(bin_PROGRAMS); \
fi; \
for p in $(sysconf_DATA); do \
$(INSTALL_DATA) $$p "$(1)/etc/$(bin_PROGRAMS)/$$p" || exit $$?; \
done; \
echo "..ok"

install: install-binPROGRAMS
	@DOLLAR_SIGN@(call install_f,$(prefix))

deb: $(bin_PROGRAMS)
	rm -rf $(debian_BUILD)
	$(MKDIR_P) $(debian_BUILD)/DEBIAN
	echo "Package: $(bin_PROGRAMS)">  $(debian_BUILD)/DEBIAN/control
	echo "Version: $(VERSION)" >> $(debian_BUILD)/DEBIAN/control
	echo "Section: base" >> $(debian_BUILD)/DEBIAN/control
	echo "Priority: optional" >> $(debian_BUILD)/DEBIAN/control
	echo "Architecture: all" >> $(debian_BUILD)/DEBIAN/control
	echo "Depends: $(DEBDEP)" >> $(debian_BUILD)/DEBIAN/control
	echo "Maintainer: Alex D <wvdial@gmail.com>" >> $(debian_BUILD)/DEBIAN/control
	echo "Description:  multithread demon, to return html(inframe) with offers. Runs on background and listen unix socket(config.xml) ready to http requests from nginx." >> $(debian_BUILD)/DEBIAN/control
	@DOLLAR_SIGN@(call install_f,$(debian_BUILD))
	for p in `find $(debian_BUILD) -type f -print | grep -v DEBIAN`; do \
	md5sum $$p | $(SED) 's/$(debian_BUILD)\///'>>$(debian_BUILD)/DEBIAN/md5sum; \
	done
	dpkg-deb --build $(debian_BUILD)
	rm -rf $(debian_BUILD)

