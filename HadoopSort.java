import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.mapreduce.lib.input.KeyValueTextInputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class HadoopSort {

  public static class SortMap
       extends Mapper<Text, Text, Text, Text>{

    private Text fKey  = new Text();
    private Text fValue  = new Text();

    public void map(LongWritable key, Text value, Context context
                    ) throws IOException, InterruptedException {
	String arr[]=value.toString().split(" ",2);
  	String newkey = arr[0];
	String newvalue = arr[1];
	
	fKey.set(newkey);
	fValue.set(newvalue);
	context.write(fKey,fValue);	
    }
  }

  public static class SortReduce
       extends Reducer<Text,Text,Text,Text> {

    public void reduce(Text key, Text values,
                       Context context
                       ) throws IOException, InterruptedException {
      context.write(key, values);
    }
  }

  public static void main(String[] args) throws Exception {
    Configuration conf = new Configuration();
    Job job = Job.getInstance(conf, "hadoop sort");
    job.setJarByClass(HadoopSort.class);
    
    job.setMapperClass(SortMap.class);
    job.setReducerClass(SortReduce.class);
    
    job.setInputFormatClass(KeyValueTextInputFormat.class);
    job.setMapOutputKeyClass(Text.class);

    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(Text.class);

    FileInputFormat.addInputPath(job, new Path(args[0]));
    FileOutputFormat.setOutputPath(job, new Path(args[1]));

    System.exit(job.waitForCompletion(true) ? 0 : 1);
  }
}
