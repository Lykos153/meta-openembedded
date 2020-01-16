DESCRIPTION = "CUPS backends, filters, and other software"
HOMEPAGE = "http://www.linuxfoundation.org/collaborate/workgroups/openprinting/cups-filters"

LICENSE = "GPLv2 & LGPLv2 & MIT & GPLv2+ & GPLv3"
LIC_FILES_CHKSUM = "file://COPYING;md5=516215fd57564996d70327db19b368ff"

SECTION = "console/utils"

DEPENDS = "cups glib-2.0 glib-2.0-native dbus dbus-glib lcms ghostscript poppler qpdf libpng"
DEPENDS_class-native = "poppler-native glib-2.0-native dbus-native pkgconfig-native gettext-native libpng-native"

SRC_URI = "http://openprinting.org/download/cups-filters/cups-filters-${PV}.tar.gz"

inherit autotools-brokensep gettext pkgconfig

EXTRA_OECONF += " --enable-ghostscript --disable-ldap \
                       --with-pdftops=hybrid --enable-imagefilters \
                       --enable-ghostscript --with-gs-path=${bindir}/gs \
                       --with-pdftops-path=${bindir}/gs \
                       --with-fontdir=${datadir}/fonts --with-rcdir=no \
                       --with-cups-rundir=${localstatedir}/run/cups \
                       --localstatedir=${localstatedir}/var \
                       --with-rcdir=no \
                       --without-php"

EXTRA_OECONF_class-native += " --with-pdftops=pdftops \
                                    --disable-avahi --disable-ghostscript \
                                    --disable-ldap \
                                    --with-png --without-jpeg --without-tiff"

BBCLASSEXTEND = "native"

PACKAGECONFIG[jpeg] = "--with-jpeg,--without-jpeg,jpeg"
PACKAGECONFIG[png] = "--with-png,--without-png,libpng"
PACKAGECONFIG[tiff] = "--with-tiff,--without-tiff,tiff"

PACKAGECONFIG ??= "${@bb.utils.contains('DISTRO_FEATURES', 'zeroconf', 'avahi', '', d)}"

PACKAGECONFIG[avahi] = "--enable-avahi,--disable-avahi,avahi"
PACKAGECONFIG[dbus] = "--enable-dbus,--disable-dbus,dbus"

DIRFILES = "1"

PACKAGES =+ "\
	${PN}-gst \
        ${PN}-data \
        "

FILES_${PN}-gst = "\
	${libdir}/cups/filter/gstopxl \
	${libdir}/cups/filter/gstoraster \
	"

FILES_${PN}-data = "\
	${datadir}/cups/data \
	"

FILES_${PN}-dbg += "\
	${libdir}/cups/filter/.debug \
	${libdir}/cups/backend/.debug \
	"

FILES_${PN} += "\
        ${libdir}/cups/filter \
        ${libdir}/cups/backend \
        ${libdir}/cups/driver \
        ${datadir}/cups/charsets \
        ${datadir}/cups/drv \
        ${datadir}/cups/mime \
        ${datadir}/cups/ppdc \
        ${datadir}/ppd/cupsfilters \
        ${datadir}/cups/braille \
        ${datadir}/cups/banners \
        ${datadir}/cups/braille/index.sh \
        ${datadir}/cups/braille/cups-braille.sh \
        ${datadir}/cups/braille/indexv3.sh \
        ${datadir}/cups/braille/indexv4.sh \
        ${datadir}/cups/banners/topsecret \
        ${datadir}/cups/banners/secret \
        ${datadir}/cups/banners/confidential \
        ${datadir}/cups/banners/unclassified \
        ${datadir}/cups/banners/form \
        ${datadir}/cups/banners/classified \
        ${datadir}/cups/banners/standard \
"

do_install_append() {
	# remove banners, braille dirs
	rm -rf ${D}${datadir}/cups/{banners,braille}

	# remove sysroot path contamination from pkgconfig file
	sed -i -e 's:${STAGING_DIR_TARGET}::' ${D}/${libdir}/pkgconfig/libcupsfilters.pc
}

RDEPENDS_${PN} += "bash"