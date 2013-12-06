在Linux上运行的命令行中英文翻译工具。


例子
--------------------------------------------------------------------------------
<pre>

lusaisai@CentOSP:~$ dict
Dict 1.0.0
Type the English word and get the English/Chinese explanations
Current avaliable dictionaries: Oxford
Press Tab for word suggestions
Press Ctrl+D to quit
> beauti
beautician      beautification  beautiful       beautifully     beautify        
> beautiful
/ 5bju:tIfl; `bjutEfEl/ adj  1 having beauty; giving pleasure to the senses or the mind 美丽的; 美观的; 美好的: a beautiful face, baby, flower, view, voice, poem, smell, morning 美妙的脸庞、婴儿、花朵、景物、声音、诗歌、气味、早晨 * beautiful weather, music, chocolate 极好的天气、音乐、巧克力.  2 very satisfactory 极其满意的: The organization was beautiful. 组织工作极佳. * What beautiful timing! 时间正合适!
> Exit Dict 1.0.0
lusaisai@CentOSP:~$ 

</pre>


词典
--------------------------------------------------------------------------------
* 词典在本地，目前只有一本牛津词典，以后会添加科林斯，21世纪，汉英等词典。


安装
--------------------------------------------------------------------------------
1. 依赖于readline和sqlite3两个库, 在CentOS和Ubuntu上安装如下。
<pre>
CentOS$ yum install sqlite-devel readline-devel
Ubuntu$ sudo apt-get install libreadline-dev libsqlite3-dev
</pre>

2. 依次跑下面的command, 默认装到home目录下的.dict目录，若要更改，把下面的./configure改成./configure 你的目录的绝对路径, 比如 ./configure /usr/share/local
<pre>
Ubuntu$ cd DictDLI/dict
Ubuntu$ ./configure
Ubuntu$ make
Ubuntu$ make install
</pre>


