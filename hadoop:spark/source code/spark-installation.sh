#!/bin/bash
wget http://apache.mirrors.pair.com/spark/spark-3.0.0-preview2/spark-3.0.0-preview2-bin-hadoop3.2.tgz
cp spark-3.0.0-preview2-bin-hadoop3.2.tgz  /exports/projects/
cd /exports/projects
tar xvf spark-3.0.0-preview2-bin-hadoop3.2.tgz
cd spark-3.0.0-preview2-bin-hadoop3.2
cp conf/spark-env.sh.template conf/spark-env.sh
echo "HADOOP_CONF_DIR=/exports/projects/hadoop-3.2.1/etc/hadoop" >>  conf/spark-env.sh
echo "YARN_CONF_DIR=/exports/projects/hadoop-3.2.1/etc/hadoop" >>  conf/spark-env.sh
mkdir /exports/projects/sparkstaging
cp conf/spark-defaults.conf.template conf/spark-defaults.conf
echo "spark.yarn.stagingDir /exports/projects/sparkstaging" >> conf/spark-defaults.conf
echo "export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-amd64/" | sudo tee -a /etc/profile.d/hadoop.sh
echo "export SPARK_HOME=/exports/projects/spark-3.0.0-preview2-bin-hadoop3.2/" | sudo tee -a /etc/profile.d/hadoop.sh