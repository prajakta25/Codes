#!/bin/bash
sudo apt install openjdk-8-jdk
ssh-keygen
cat .ssh/id_rsa.pub >> .ssh/authorized_keys
chmod 600 .ssh/authorized_keys
chmod 700 .ssh
wget http://apache.mirrors.hoobly.com/hadoop/common/hadoop-3.2.1/hadoop-3.2.1.tar.gz
mv hadoop-3.2.1.tar.gz /exports/projects/
cd /exports/projects
tar xzvf hadoop-3.2.1.tar.gz
cd hadoop-3.2.1
echo "export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-amd64" >> etc/hadoop/hadoop-env.sh
echo "export HADOOP_HOME=/exports/projects/hadoop-3.2.1" | sudo tee -a /etc/profile.d/hadoop.sh
echo "export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-amd64" | sudo tee -a /etc/profile.d/hadoop.sh