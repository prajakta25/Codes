import org.apache.spark.SparkConf;

import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import scala.Tuple2;
import java.util.*;
import java.io.*;
import java.util.Arrays;
import org.apache.spark.api.java.function.*;


public class SparkSort {
    public static void main(String[] args) {
        SparkConf sparkConf = new SparkConf().setAppName("Spark Sort");
        JavaSparkContext sparkContext = new JavaSparkContext(sparkConf);
        sparkConf.set("spark.hadoop.validateOutputSpecs", "false");
        sparkConf.set("spark.hadoop.orc.overwrite.output.file", "true");
        String nodeip = args[0];
        String inPath = args[1];
        String outPath = args[2];
        String in = nodeip+inPath;
        String out = nodeip+outPath;
        JavaRDD<String> textFile = sparkContext.textFile(in);
        JavaPairRDD<String,String> keyData = textFile.mapToPair(s -> {
                String arr = s.substring(0,10);
                return new Tuple2<String,String>(arr,s);
        });
        JavaPairRDD<String,String> sortedVal = keyData.sortByKey();
        sortedVal.map(x->x._2.toString().replaceAll("\\s+$","\r\n")).saveAsTextFile(out);
    }
}