# HIREDIS
------------------------------------------
hiredus是redis官方推荐的一个c++客户端库<br>
github: https://github.com/redis/hiredis<br>
但是该库目前的版本不支持在windows上编译<br>
<br>

# RedisExample
------------------------------------------
在hiredus的源码基础上,把部分不能在windows上编译的代码改为跨平台<br>
提供了一种异步操作redis的方法,把hiredus的操作部分放到一个单独的线程里,主线程通过单生产者单消费者的队列与该线程交互<br>
部分语法用到了c++11 所以编译需要支持c++11才行<br>

# compile on windwos
------------------------------------------
RedisExample.sln

# compile on linux
------------------------------------------
先编译hiredis库:<br>
cd RedisExample/Redis/hiredis<br>
make<br>

在编译RedisExample:<br>
cd RedisExample<br>
make<br>

运行:<br>
RedisExample redisIP redisPort<br>
输入测试命令<br>
