#!/usr/bin/bash
sudo yum -y update
sudo yum -y groupinstall "Development Tools"
sudo yum -y install cmake gcc-c++ gcc make zlib-devel pcre-devel libicu-devel libgcrypt-devel vim
sudo yum -y install epel-release
sudo yum -y install nginx
# firewall-cmd --permanent --zone=public --add-service=http  # allow http
# firewall-cmd --permanent --zone=public --add-service=https # allow https
# firewall-cmd --reload
sudo setenforce permissive  # set to grant permissions
sudo yum -y install http://dev.mysql.com/get/mysql-community-release-el7-5.noarch.rpm
sudo yum -y update
sudo yum -y install mysql-community-server
sudo yum -y install mysql-community-devel
