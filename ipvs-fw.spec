%define		kversion %(uname -r)

Name:		kmod-ipvs-fw-%{kversion}
Version:	%{version}
Release:	%{release}%{?dist}
Summary:	Fixed weighted scheduler for IP Virtual Server

Group:		System Environment/Kernel
License:	GPLv2
URL:		https://github.com/ivoronin/ipvs-fw
Source0:	ipvs-fw-%{version}.tar.gz

Requires:	kernel = %(echo %{kversion} | sed -re 's/\.[^.]+$//')
BuildRequires:	redhat-rpm-config

%description
Fixed weighted scheduler for IP Virtual Server - the highest weight
non-overloaded destination is only used when other destination(s) are given
lower weight values.

If there are several destinations with the highest weight, the first one is used.
%prep
%setup -n ipvs-fw-%{version}

%build
make

%install
make modules_install INSTALL_MOD_PATH=%{buildroot}

%define _unpackaged_files_terminate_build 0
%files
/lib/modules/%{kversion}/extra/ip_vs_fw.ko

%post
depmod
