## 安装编译PostgreSQL源码
下载并解压
```Shell
wget https://ftp.postgresql.org/pub/source/v16.6/postgresql-16.6.tar.gz

tar -xvzf postgresql-16.6.tar.gz
```
安装依赖
```Shell
apt install openssh-server net-tools make autoconf automake libtool pkg-config g++ flex bison libreadline-dev zlib1g-dev libssl-dev libxml2-dev libxslt1-dev libperl-dev python3-dev python3-pip libpython3-dev libkrb5-dev krb5-multidev libicu-dev libldap2-dev libsasl2-dev libgssapi-krb5-2 libedit-dev libossp-uuid-dev gettext
```
开启root登录
```Shell
sudo vim /etc/ssh/sshd_config
```
将PermitRootLogin prohibit-password 改为 PermitRootLogin yes
```Shell
/etc/init.d/ssh restart
sudo passwd -u root
sudo passwd root
```

简单编译
```Shell
./configure
make
make install
adduser postgres
mkdir -p /usr/local/pgsql/data
chown postgres /usr/local/pgsql/data
su - postgres
/usr/local/pgsql/bin/initdb -D /usr/local/pgsql/data
/usr/local/pgsql/bin/pg_ctl -D /usr/local/pgsql/data -l logfile start
/usr/local/pgsql/bin/createdb test
/usr/local/pgsql/bin/psql test
```

我们需要对postgreSQL进行debug，所以需要一些设置，下面是官方介绍