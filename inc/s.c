前序：
Sqlite3 的确很好用。小巧、速度快。但是因为非微软的产品，帮助文档总觉得不够。这些天再次研究它，又有一些收获，这里把我对 sqlite3 的研究列出来，以备忘记。
这里要注明，我是一个跨平台专注者，并不喜欢只用 windows 平台。我以前的工作就是为 unix 平台写代码。下面我所写的东西，虽然没有验证，但是我已尽量不使用任何 windows 的东西，只使用标准 C 或标准C++。但是，我没有尝试过在别的系统、别的编译器下编译，因此下面的叙述如果不正确，则留待以后修改。
下面我的代码仍然用 VC 编写，因为我觉得VC是一个很不错的IDE，可以加快代码编写速度（例如配合 Vassist ）。下面我所说的编译环境，是VC2003。如果读者觉得自己习惯于 unix 下用 vi 编写代码速度较快，可以不用管我的说明，只需要符合自己习惯即可，因为我用的是标准 C 或 C++ 。不会给任何人带来不便。

一、版本
从 www.sqlite.org 网站可下载到最新的 sqlite 代码和编译版本。我写此文章时，最新代码是 3.3.17 版本。
很久没有去下载 sqlite 新代码，因此也不知道 sqlite 变化这么大。以前很多文件，现在全部合并成一个 sqlite3.c 文件。如果单独用此文件，是挺好的，省去拷贝一堆文件还担心有没有遗漏。但是也带来一个问题：此文件太大，快接近7万行代码，VC开它整个机器都慢下来了。如果不需要改它代码，也就不需要打开 sqlite3.c 文件，机器不会慢。但是，下面我要写通过修改 sqlite 代码完成加密功能，那时候就比较痛苦了。如果个人水平较高，建议用些简单的编辑器来编辑，例如 UltraEdit 或 Notepad 。速度会快很多。

二、基本编译
这个不想多说了，在 VC 里新建 dos 控制台空白工程，把 sqlite3.c 和 sqlite3.h 添加到工程，再新建一个 main.cpp 文件。在里面写:

extern "C"
{
#include "./sqlite3.h"
};
int main( int , char** )
{
return 0;
}

为什么要 extern “C” ？如果问这个问题，我不想说太多，这是C++的基础。要在 C++ 里使用一段 C 的代码，必须要用 extern “C” 括起来。C++跟 C虽然语法上有重叠，但是它们是两个不同的东西，内存里的布局是完全不同的，在C++编译器里不用extern “C”括起C代码，会导致编译器不知道该如何为 C 代码描述内存布局。
可能在 sqlite3.c 里人家已经把整段代码都 extern “C” 括起来了，但是你遇到一个 .c 文件就自觉的再括一次，也没什么不好。
基本工程就这样建立起来了。编译，可以通过。但是有一堆的 warning。可以不管它。

三、SQLITE操作入门
sqlite提供的是一些C函数接口，你可以用这些函数操作数据库。通过使用这些接口，传递一些标准 sql 语句（以 char * 类型）给 sqlite 函数，sqlite 就会为你操作数据库。
sqlite 跟MS的access一样是文件型数据库，就是说，一个数据库就是一个文件，此数据库里可以建立很多的表，可以建立索引、触发器等等，但是，它实际上得到的就是一个文件。备份这个文件就备份了整个数据库。
sqlite 不需要任何数据库引擎，这意味着如果你需要 sqlite 来保存一些用户数据，甚至都不需要安装数据库(如果你做个小软件还要求人家必须装了sqlserver 才能运行，那也太黑心了)。
下面开始介绍数据库基本操作。
1 基本流程（1）关键数据结构
sqlite 里最常用到的是sqlite3 * 类型。从数据库打开开始，sqlite就要为这个类型准备好内存，直到数据库关闭，整个过程都需要用到这个类型。当数据库打开时开始，这个类型的变量就代表了你要操作的数据库。下面再详细介绍。
（2）打开数据库
int sqlite3_open( 文件名, sqlite3 ** );
用这个函数开始数据库操作。
需要传入两个参数，一是数据库文件名，比如：c:\\DongChunGuang_Database.db。
文件名不需要一定存在，如果此文件不存在，sqlite 会自动建立它。如果它存在，就尝试把它当数据库文件来打开。
sqlite3 ** 参数即前面提到的关键数据结构。这个结构底层细节如何，你不要关它。
函数返回值表示操作是否正确，如果是SQLITE_OK 则表示操作正常。相关的返回值sqlite定义了一些宏。具体这些宏的含义可以参考 sqlite3.h 文件。里面有详细定义（顺便说一下，sqlite3 的代码注释率自称是非常高的，实际上也的确很高。只要你会看英文，sqlite 可以让你学到不少东西）。
下面介绍关闭数据库后，再给一段参考代码。
（3）关闭数据库
int sqlite3_close(sqlite3 *);
前面如果用 sqlite3_open 开启了一个数据库，结尾时不要忘了用这个函数关闭数据库。
下面给段简单的代码：
extern "C"
{
#include "./sqlite3.h"
};
int main( int , char** )
{
   sqlite3 * db = NULL; //声明sqlite关键结构指针
   int result;

//打开数据库
//需要传入 db 这个指针的指针，因为 sqlite3_open 函数要为这个指针分配内存，还要让db指针指向这个内存区
   result = sqlite3_open( “c:\\Dcg_database.db”, &db );
   if( result != SQLITE_OK )
   {
    //数据库打开失败
return -1;
}
//数据库操作代码
//…

//数据库打开成功
//关闭数据库
sqlite3_close( db );
return 0;
}
这就是一次数据库操作过程。


2 SQL语句操作
本节介绍如何用sqlite 执行标准 sql 语法。

（1）执行sql语句
int sqlite3_exec(sqlite3*, const char *sql, sqlite3_callback, void *,  char **errmsg );
这就是执行一条 sql 语句的函数。
第1个参数不再说了，是前面open函数得到的指针。说了是关键数据结构。
第2个参数const char *sql 是一条 sql 语句，以\0结尾。
第3个参数sqlite3_callback 是回调，当这条语句执行之后，sqlite3会去调用你提供的这个函数。（什么是回调函数，自己找别的资料学习）
第4个参数void * 是你所提供的指针，你可以传递任何一个指针参数到这里，这个参数最终会传到回调函数里面，如果不需要传递指针给回调函数，可以填NULL。等下我们再看回调函数的写法，以及这个参数的使用。
第 5个参数char ** errmsg 是错误信息。注意是指针的指针。sqlite3里面有很多固定的错误信息。执行 sqlite3_exec 之后，执行失败时可以查阅这个指针（直接 printf(“%s\n”,errmsg)）得到一串字符串信息，这串信息告诉你错在什么地方。sqlite3_exec函数通过修改你传入的指针的指针，把你提供的指针指向错误提示信息，这样sqlite3_exec函数外面就可以通过这个 char*得到具体错误提示。
说明：通常，sqlite3_callback 和它后面的 void * 这两个位置都可以填 NULL。填NULL表示你不需要回调。比如你做 insert 操作，做 delete 操作，就没有必要使用回调。而当你做 select 时，就要使用回调，因为 sqlite3 把数据查出来，得通过回调告诉你查出了什么数据。
（2）exec 的回调
typedef int (*sqlite3_callback)(void*,int,char**, char**);
你的回调函数必须定义成上面这个函数的类型。下面给个简单的例子：
//sqlite3的回调函数       
// sqlite 每查到一条记录，就调用一次这个回调
int LoadMyInfo( void * para, int n_column, char ** column_value, char ** column_name )
{
   //para是你在 sqlite3_exec 里传入的 void * 参数
   //通过para参数，你可以传入一些特殊的指针（比如类指针、结构指针），然后在这里面强制转换成对应的类型（这里面是void*类型，必须强制转换成你的类型才可用）。然后操作这些数据
   //n_column是这一条记录有多少个字段 (即这条记录有多少列)
   // char ** column_value 是个关键值，查出来的数据都保存在这里，它实际上是个1维数组（不要以为是2维数组），每一个元素都是一个 char * 值，是一个字段内容（用字符串来表示，以\0结尾）

   //char ** column_name 跟 column_value是对应的，表示这个字段的字段名称       

    //这里，我不使用 para 参数。忽略它的存在.

    int i;

printf( “记录包含 %d 个字段\n”, n_column );
for( i = 0 ; i < n_column; i ++ )
{
    printf( “字段名:%s  ß> 字段值:%s\n”,  column_name[i], column_value[i] );
}
printf( “------------------\n“ );         
return 0;
}

int main( int , char ** )
{
    sqlite3 * db;
    int result;
    char * errmsg = NULL;

    result = sqlite3_open( “c:\\Dcg_database.db”, &db );
    if( result != SQLITE_OK )
    {
        //数据库打开失败
return -1;
}

//数据库操作代码
//创建一个测试表，表名叫 MyTable_1，有2个字段： ID 和 name。其中ID是一个自动增加的类型，以后insert时可以不去指定这个字段，它会自己从0开始增加
result = sqlite3_exec( db, “create table MyTable_1( ID integer primary key autoincrement, name nvarchar(32) )”, NULL, NULL, errmsg );
if(result != SQLITE_OK )
{
    printf( “创建表失败，错误码:%d，错误原因:%s\n”, result, errmsg );
}

//插入一些记录
result = sqlite3_exec( db, “insert into MyTable_1( name ) values ( ‘走路’ )”, 0, 0, errmsg );
if(result != SQLITE_OK )
{
    printf( “插入记录失败，错误码:%d，错误原因:%s\n”, result, errmsg );
}

result = sqlite3_exec( db, “insert into MyTable_1( name ) values ( ‘骑单车’ )”, 0, 0, errmsg );
if(result != SQLITE_OK )
{
    printf( “插入记录失败，错误码:%d，错误原因:%s\n”, result, errmsg );
}

result = sqlite3_exec( db, “insert into MyTable_1( name ) values ( ‘坐汽车’ )”, 0, 0, errmsg );
if(result != SQLITE_OK )
{
    printf( “插入记录失败，错误码:%d，错误原因:%s\n”, result, errmsg );
}

//开始查询数据库
result = sqlite3_exec( db, “select * from MyTable_1”, LoadMyInfo, NULL, errmsg );

//关闭数据库
sqlite3_close( db );
return 0;
}

通过上面的例子，应该可以知道如何打开一个数据库，如何做数据库基本操作。
有这些知识，基本上可以应付很多数据库操作了。

（3）不使用回调查询数据库
上面介绍的 sqlite3_exec 是使用回调来执行 select 操作。还有一个方法可以直接查询而不需要回调。但是，我个人感觉还是回调好，因为代码可以更加整齐，只不过用回调很麻烦，你得声明一个函数，如果这个函数是类成员函数，你还不得不把它声明成 static 的（要问为什么？这又是C++基础了。C++成员函数实际上隐藏了一个参数：this，C++调用类的成员函数的时候，隐含把类指针当成函数的第一个参数传递进去。结果，这造成跟前面说的 sqlite 回调函数的参数不相符。只有当把成员函数声明成 static 时，它才没有多余的隐含的this参数）。
虽然回调显得代码整齐，但有时候你还是想要非回调的 select 查询。这可以通过 sqlite3_get_table 函数做到。
int sqlite3_get_table(sqlite3*, const char *sql, char ***resultp, int *nrow, int *ncolumn, char **errmsg );
第1个参数不再多说，看前面的例子。
第2个参数是 sql 语句，跟 sqlite3_exec 里的 sql 是一样的。是一个很普通的以\0结尾的char *字符串。
第3个参数是查询结果，它依然一维数组（不要以为是二维数组，更不要以为是三维数组）。它内存布局是：第一行是字段名称，后面是紧接着是每个字段的值。下面用例子来说事。
第4个参数是查询出多少条记录（即查出多少行）。
第5个参数是多少个字段（多少列）。
第6个参数是错误信息，跟前面一样，这里不多说了。
下面给个简单例子:
int main( int , char ** )
{
   sqlite3 * db;
   int result;
   char * errmsg = NULL;
   char **dbResult; //是 char ** 类型，两个*号
   int nRow, nColumn;
   int i , j;
   int index;

   result = sqlite3_open( “c:\\Dcg_database.db”, &db );
   if( result != SQLITE_OK )
   {
        //数据库打开失败
        return -1;
   }

   //数据库操作代码
   //假设前面已经创建了 MyTable_1 表
   //开始查询，传入的 dbResult 已经是 char **，这里又加了一个 & 取地址符，传递进去的就成了 char ***
   result = sqlite3_get_table( db, “select * from MyTable_1”, &dbResult, &nRow, &nColumn, &errmsg );
   if( SQLITE_OK == result )
   {
        //查询成功
        index = nColumn; //前面说过 dbResult 前面第一行数据是字段名称，从 nColumn 索引开始才是真正的数据
        printf( “查到%d条记录\n”, nRow );

        for(  i = 0; i < nRow ; i++ )
        {
             printf( “第 %d 条记录\n”, i+1 );
             for( j = 0 ; j < nColumn; j++ )
             {
                  printf( “字段名:%s  ß> 字段值:%s\n”,  dbResult[j], dbResult [index] );
                  ++index; // dbResult 的字段值是连续的，从第0索引到第 nColumn - 1索引都是字段名称，从第 nColumn 索引开始，后面都是字段值，它把一个二维的表（传统的行列表示法）用一个扁平的形式来表示
             }
             printf( “-------\n” );
        }
   }

   //到这里，不论数据库查询是否成功，都释放 char** 查询结果，使用 sqlite 提供的功能来释放
   sqlite3_free_table( dbResult );

   //关闭数据库
   sqlite3_close( db );
   return 0;
}

到这个例子为止，sqlite3 的常用用法都介绍完了。
用以上的方法，再配上 sql 语句，完全可以应付绝大多数数据库需求。
但有一种情况，用上面方法是无法实现的：需要insert、select 二进制。当需要处理二进制数据时，上面的方法就没办法做到。下面这一节说明如何插入二进制数据


3 操作二进制
sqlite 操作二进制数据需要用一个辅助的数据类型：sqlite3_stmt * 。
这个数据类型记录了一个“sql语句”。为什么我把 “sql语句” 用双引号引起来？因为你可以把 sqlite3_stmt * 所表示的内容看成是 sql语句，但是实际上它不是我们所熟知的sql语句。它是一个已经把sql语句解析了的、用sqlite自己标记记录的内部数据结构。
正因为这个结构已经被解析了，所以你可以往这个语句里插入二进制数据。当然，把二进制数据插到 sqlite3_stmt 结构里可不能直接 memcpy ，也不能像 std::string 那样用 + 号。必须用 sqlite 提供的函数来插入。

（1）写入二进制
下面说写二进制的步骤。
要插入二进制，前提是这个表的字段的类型是 blob 类型。我假设有这么一张表：
create table Tbl_2( ID integer, file_content  blob )
首先声明
sqlite3_stmt * stat;
然后，把一个 sql 语句解析到 stat 结构里去：
sqlite3_prepare( db, “insert into Tbl_2( ID, file_content) values( 10, ? )”, -1, &stat, 0 );
上面的函数完成 sql 语句的解析。第一个参数跟前面一样，是个 sqlite3 * 类型变量，第二个参数是一个 sql 语句。
这个 sql 语句特别之处在于 values 里面有个 ? 号。在sqlite3_prepare函数里，?号表示一个未定的值，它的值等下才插入。
第三个参数我写的是-1，这个参数含义是前面 sql 语句的长度。如果小于0，sqlite会自动计算它的长度（把sql语句当成以\0结尾的字符串）。
第四个参数是 sqlite3_stmt 的指针的指针。解析以后的sql语句就放在这个结构里。
第五个参数我也不知道是干什么的。为0就可以了。
如果这个函数执行成功（返回值是 SQLITE_OK 且 stat 不为NULL ），那么下面就可以开始插入二进制数据。
sqlite3_bind_blob( stat, 1, pdata, (int)(length_of_data_in_bytes), NULL ); // pdata为数据缓冲区，length_of_data_in_bytes为数据大小，以字节为单位
这个函数一共有5个参数。
第1个参数：是前面prepare得到的 sqlite3_stmt * 类型变量。
第 2个参数：?号的索引。前面prepare的sql语句里有一个?号，假如有多个?号怎么插入？方法就是改变 bind_blob 函数第2个参数。这个参数我写1，表示这里插入的值要替换 stat 的第一个?号（这里的索引从1开始计数，而非从0开始）。如果你有多个?号，就写多个 bind_blob 语句，并改变它们的第2个参数就替换到不同的?号。如果有?号没有替换，sqlite为它取值null。
第3个参数：二进制数据起始指针。
第4个参数：二进制数据的长度，以字节为单位。
第5个参数：是个析够回调函数，告诉sqlite当把数据处理完后调用此函数来析够你的数据。这个参数我还没有使用过，因此理解也不深刻。但是一般都填NULL，需要释放的内存自己用代码来释放。
bind完了之后，二进制数据就进入了你的“sql语句”里了。你现在可以把它保存到数据库里：
int result = sqlite3_step( stat );
通过这个语句，stat 表示的sql语句就被写到了数据库里。
最后，要把 sqlite3_stmt 结构给释放：
sqlite3_finalize( stat ); //把刚才分配的内容析构掉

（2）读出二进制
下面说读二进制的步骤。
跟前面一样，先声明 sqlite3_stmt * 类型变量：
sqlite3_stmt * stat;
然后，把一个 sql 语句解析到 stat 结构里去：
sqlite3_prepare( db, “select * from Tbl_2”, -1, &stat, 0 );
当 prepare 成功之后（返回值是 SQLITE_OK ），开始查询数据。
int result = sqlite3_step( stat );
这一句的返回值是SQLITE_ROW 时表示成功（不是 SQLITE_OK ）。
你可以循环执行sqlite3_step 函数，一次step查询出一条记录。直到返回值不为 SQLITE_ROW 时表示查询结束。
然后开始获取第一个字段：ID 的值。ID是个整数，用下面这个语句获取它的值：
int id = sqlite3_column_int( stat, 0 ); //第2个参数表示获取第几个字段内容，从0开始计算，因为我的表的ID字段是第一个字段，因此这里我填0

下面开始获取 file_content 的值，因为 file_content 是二进制，因此我需要得到它的指针，还有它的长度：
const void * pFileContent = sqlite3_column_blob( stat, 1 );
int len = sqlite3_column_bytes( stat, 1 );
这样就得到了二进制的值。
把 pFileContent 的内容保存出来之后，不要忘了释放 sqlite3_stmt 结构：
sqlite3_finalize( stat ); //把刚才分配的内容析构掉

（3）重复使用 sqlite3_stmt 结构
如果你需要重复使用 sqlite3_prepare 解析好的 sqlite3_stmt 结构，需要用函数： sqlite3_reset。
result = sqlite3_reset(stat);
这样， stat 结构又成为 sqlite3_prepare 完成时的状态，你可以重新为它 bind 内容。
4 事务处理
sqlite 是支持事务处理的。如果你知道你要同步删除很多数据，不仿把它们做成一个统一的事务。
通常一次 sqlite3_exec 就是一次事务，如果你要删除1万条数据，sqlite就做了1万次：开始新事务->删除一条数据->提交事务->开始新事务->… 的过程。这个操作是很慢的。因为时间都花在了开始事务、提交事务上。
你可以把这些同类操作做成一个事务，这样如果操作错误，还能够回滚事务。
事务的操作没有特别的接口函数，它就是一个普通的 sql 语句而已：
分别如下：
int result;  
result = sqlite3_exec( db, "begin transaction", 0, 0, &zErrorMsg ); //开始一个事务
result = sqlite3_exec( db, "commit transaction", 0, 0, &zErrorMsg ); //提交事务

result = sqlite3_exec( db, "rollback transaction", 0, 0, &zErrorMsg ); //回滚事务
四、C/C++开发接口简介1 总览
SQLite3是SQLite一个全新的版本,它虽然是在SQLite 2.8.13的代码基础之上开发的,但是使用了和之前的版本不兼容的数据库格式和API. SQLite3是为了满足以下的需求而开发的:
支持UTF-16编码. 用户自定义的文本排序方法. 可以对BLOBs字段建立索引.
因此为了支持这些特性我改变了数据库的格式,建立了一个与之前版本不兼容的3.0版. 至于其他的兼容性的改变,例如全新的API等等,都将在理论介绍之后向你说明,这样可以使你最快的一次性摆脱兼容性问题.
3.0版的和2.X版的API非常相似,但是有一些重要的改变需要注意. 所有API接口函数和数据结构的前缀都由"sqlite_"改为了"sqlite3_". 这是为了避免同时使用SQLite 2.X和SQLite 3.0这两个版本的时候发生链接冲突.
由于对于C语言应该用什么数据类型来存放UTF-16编码的字符串并没有一致的规范. 因此SQLite使用了普通的void* 类型来指向UTF-16编码的字符串. 客户端使用过程中可以把void*映射成适合他们的系统的任何数据类型.
2 C/C++接口
SQLite 3.0一共有83个API函数,此外还有一些数据结构和预定义(#defines). (完整的API介绍请参看另一份文档.) 不过你们可以放心,这些接口使用起来不会像它的数量所暗示的那么复杂. 最简单的程序仍然使用三个函数就可以完成: sqlite3_open(), sqlite3_exec(), 和 sqlite3_close(). 要是想更好的控制数据库引擎的执行,可以使用提供的sqlite3_prepare()函数把SQL语句编译成字节码,然后在使用 sqlite3_step()函数来执行编译后的字节码. 以sqlite3_column_开头的一组API函数用来获取查询结果集中的信息. 许多接口函数都是成对出现的,同时有UTF-8和UTF-16两个版本. 并且提供了一组函数用来执行用户自定义的SQL函数和文本排序函数.
（1）如何打开关闭数据库
   typedef struct sqlite3 sqlite3;
   int sqlite3_open(const char*, sqlite3**);
   int sqlite3_open16(const void*, sqlite3**);
   int sqlite3_close(sqlite3*);
   const char *sqlite3_errmsg(sqlite3*);
   const void *sqlite3_errmsg16(sqlite3*);
   int sqlite3_errcode(sqlite3*);
sqlite3_open() 函数返回一个整数错误代码,而不是像第二版中一样返回一个指向sqlite3结构体的指针. sqlite3_open() 和 sqlite3_open16() 的不同之处在于sqlite3_open16() 使用UTF-16编码(使用本地主机字节顺序)传递数据库文件名. 如果要创建新数据库, sqlite3_open16() 将内部文本转换为UTF-16编码, 反之sqlite3_open() 将文本转换为UTF-8编码.
打开或者创建数据库的命令会被缓存,直到这个数据库真正被调用的时候才会被执行. 而且允许使用PRAGMA声明来设置如本地文本编码或默认内存页面大小等选项和参数.
sqlite3_errcode() 通常用来获取最近调用的API接口返回的错误代码. sqlite3_errmsg() 则用来得到这些错误代码所对应的文字说明. 这些错误信息将以 UTF-8 的编码返回,并且在下一次调用任何SQLite API函数的时候被清除. sqlite3_errmsg16() 和 sqlite3_errmsg() 大体上相同,除了返回的错误信息将以 UTF-16 本机字节顺序编码.
SQLite3的错误代码相比SQLite2没有任何的改变,它们分别是:
#define SQLITE_OK           0   /* Successful result */
#define SQLITE_ERROR        1   /* SQL error or missing database */
#define SQLITE_INTERNAL     2   /* An internal logic error in SQLite */
#define SQLITE_PERM         3   /* Access permission denied */
#define SQLITE_ABORT        4   /* Callback routine requested an abort */
#define SQLITE_BUSY         5   /* The database file is locked */
#define SQLITE_LOCKED       6   /* A table in the database is locked */
#define SQLITE_NOMEM        7   /* A malloc() failed */
#define SQLITE_READONLY     8   /* Attempt to write a readonly database */
#define SQLITE_INTERRUPT    9   /* Operation terminated by sqlite_interrupt() */
#define SQLITE_IOERR       10   /* Some kind of disk I/O error occurred */
#define SQLITE_CORRUPT     11   /* The database disk image is malformed */
#define SQLITE_NOTFOUND    12   /* (Internal Only) Table or record not found */
#define SQLITE_FULL        13   /* Insertion failed because database is full */
#define SQLITE_CANTOPEN    14   /* Unable to open the database file */
#define SQLITE_PROTOCOL    15   /* Database lock protocol error */
#define SQLITE_EMPTY       16   /* (Internal Only) Database table is empty */
#define SQLITE_SCHEMA      17   /* The database schema changed */
#define SQLITE_TOOBIG      18   /* Too much data for one row of a table */
#define SQLITE_CONSTRAINT  19   /* Abort due to contraint violation */
#define SQLITE_MISMATCH    20   /* Data type mismatch */
#define SQLITE_MISUSE      21   /* Library used incorrectly */
#define SQLITE_NOLFS       22   /* Uses OS features not supported on host */
#define SQLITE_AUTH        23   /* Authorization denied */
#define SQLITE_ROW         100  /* sqlite_step() has another row ready */
#define SQLITE_DONE        101  /* sqlite_step() has finished executing */

（2）执行 SQL 语句
typedef int (*sqlite_callback)(void*,int,char**, char**);
int sqlite3_exec(sqlite3*, const char *sql, sqlite_callback, void*, char**);
sqlite3_exec 函数依然像它在SQLite2中一样承担着很多的工作. 该函数的第二个参数中可以编译和执行零个或多个SQL语句. 查询的结果返回给回调函数. 更多地信息可以查看API 参考.
在SQLite3里,sqlite3_exec一般是被准备SQL语句接口封装起来使用的.
typedef struct sqlite3_stmt sqlite3_stmt;
int sqlite3_prepare(sqlite3*, const char*, int, sqlite3_stmt**, const char**);
int sqlite3_prepare16(sqlite3*, const void*, int, sqlite3_stmt**, const void**);
int sqlite3_finalize(sqlite3_stmt*);
int sqlite3_reset(sqlite3_stmt*);
sqlite3_prepare 接口把一条SQL语句编译成字节码留给后面的执行函数. 使用该接口访问数据库是当前比较好的的一种方法.
sqlite3_prepare() 处理的SQL语句应该是UTF-8编码的. 而sqlite3_prepare16() 则要求是UTF-16编码的. 输入的参数中只有第一个SQL语句会被编译. 第四个参数则用来指向输入参数中下一个需要编译的SQL语句存放的SQLite statement对象的指针, 任何时候如果调用 sqlite3_finalize() 将销毁一个准备好的SQL声明. 在数据库关闭之前，所有准备好的声明都必须被释放销毁. sqlite3_reset() 函数用来重置一个SQL声明的状态，使得它可以被再次执行.
SQL 声明可以包含一些型如"?" 或 "?nnn" 或 ":aaa"的标记， 其中"nnn" 是一个整数，"aaa" 是一个字符串. 这些标记代表一些不确定的字符值（或者说是通配符），可以在后面用sqlite3_bind 接口来填充这些值. 每一个通配符都被分配了一个编号（由它在SQL声明中的位置决定，从1开始），此外也可以用 "nnn" 来表示 "?nnn" 这种情况. 允许相同的通配符在同一个SQL声明中出现多次, 在这种情况下所有相同的通配符都会被替换成相同的值. 没有被绑定的通配符将自动取NULL值.
int sqlite3_bind_blob(sqlite3_stmt*, int, const void*, int n, void(*)(void*));
int sqlite3_bind_double(sqlite3_stmt*, int, double);
int sqlite3_bind_int(sqlite3_stmt*, int, int);
int sqlite3_bind_int64(sqlite3_stmt*, int, long long int);
int sqlite3_bind_null(sqlite3_stmt*, int);
int sqlite3_bind_text(sqlite3_stmt*, int, const char*, int n, void(*)(void*));
int sqlite3_bind_text16(sqlite3_stmt*, int, const void*, int n, void(*)(void*));
int sqlite3_bind_value(sqlite3_stmt*, int, const sqlite3_value*);
以上是 sqlite3_bind 所包含的全部接口，它们是用来给SQL声明中的通配符赋值的. 没有绑定的通配符则被认为是空值. 绑定上的值不会被sqlite3_reset()函数重置. 但是在调用了sqlite3_reset()之后所有的通配符都可以被重新赋值.
在SQL声明准备好之后(其中绑定的步骤是可选的), 需要调用以下的方法来执行:
int sqlite3_step(sqlite3_stmt*);
如果SQL返回了一个单行结果集，sqlite3_step() 函数将返回 SQLITE_ROW , 如果SQL语句执行成功或者正常将返回 SQLITE_DONE , 否则将返回错误代码. 如果不能打开数据库文件则会返回 SQLITE_BUSY . 如果函数的返回值是 SQLITE_ROW, 那么下边的这些方法可以用来获得记录集行中的数据:
const void *sqlite3_column_blob(sqlite3_stmt*, int iCol);
int sqlite3_column_bytes(sqlite3_stmt*, int iCol);
int sqlite3_column_bytes16(sqlite3_stmt*, int iCol);
int sqlite3_column_count(sqlite3_stmt*);
const char *sqlite3_column_decltype(sqlite3_stmt *, int iCol);
const void *sqlite3_column_decltype16(sqlite3_stmt *, int iCol);
double sqlite3_column_double(sqlite3_stmt*, int iCol);
int sqlite3_column_int(sqlite3_stmt*, int iCol);
long long int sqlite3_column_int64(sqlite3_stmt*, int iCol);
const char *sqlite3_column_name(sqlite3_stmt*, int iCol);
const void *sqlite3_column_name16(sqlite3_stmt*, int iCol);
const unsigned char *sqlite3_column_text(sqlite3_stmt*, int iCol);
const void *sqlite3_column_text16(sqlite3_stmt*, int iCol);
int sqlite3_column_type(sqlite3_stmt*, int iCol);
sqlite3_column_count() 函数返回结果集中包含的列数. sqlite3_column_count() 可以在执行了 sqlite3_prepare()之后的任何时刻调用. sqlite3_data_count()除了必需要在sqlite3_step()之后调用之外，其他跟sqlite3_column_count() 大同小异. 如果调用sqlite3_step() 返回值是 SQLITE_DONE 或者一个错误代码, 则此时调用sqlite3_data_count() 将返回 0 ，然而 sqlite3_column_count() 仍然会返回结果集中包含的列数.
返回的记录集通过使用其它的几个 sqlite3_column_***() 函数来提取, 所有的这些函数都把列的编号作为第二个参数. 列编号从左到右以零起始. 请注意它和之前那些从1起始的参数的不同.
sqlite3_column_type()函数返回第N列的值的数据类型. 具体的返回值如下:
#define SQLITE_INTEGER  1
#define SQLITE_FLOAT    2
#define SQLITE_TEXT     3
#define SQLITE_BLOB     4
#define SQLITE_NULL     5
sqlite3_column_decltype() 则用来返回该列在 CREATE TABLE 语句中声明的类型. 它可以用在当返回类型是空字符串的时候. sqlite3_column_name() 返回第N列的字段名. sqlite3_column_bytes() 用来返回 UTF-8 编码的BLOBs列的字节数或者TEXT字符串的字节数. sqlite3_column_bytes16() 对于BLOBs列返回同样的结果，但是对于TEXT字符串则按 UTF-16 的编码来计算字节数. sqlite3_column_blob() 返回 BLOB 数据. sqlite3_column_text() 返回 UTF-8 编码的 TEXT 数据. sqlite3_column_text16() 返回 UTF-16 编码的 TEXT 数据. sqlite3_column_int() 以本地主机的整数格式返回一个整数值. sqlite3_column_int64() 返回一个64位的整数. 最后, sqlite3_column_double() 返回浮点数.
不一定非要按照sqlite3_column_type()接口返回的数据类型来获取数据. 数据类型不同时软件将自动转换.
（3）用户自定义函数
可以使用以下的方法来创建用户自定义的SQL函数:
typedef struct sqlite3_value sqlite3_value;
int sqlite3_create_function(
     sqlite3 *,
     const char *zFunctionName,
     int nArg,
     int eTextRep,
     void*,
     void (*xFunc)(sqlite3_context*,int,sqlite3_value**),
     void (*xStep)(sqlite3_context*,int,sqlite3_value**),
     void (*xFinal)(sqlite3_context*)
   );

   int sqlite3_create_function16(
     sqlite3*,
     const void *zFunctionName,
     int nArg,
     int eTextRep,
     void*,
     void (*xFunc)(sqlite3_context*,int,sqlite3_value**),
     void (*xStep)(sqlite3_context*,int,sqlite3_value**),
     void (*xFinal)(sqlite3_context*)
   );
   #define SQLITE_UTF8     1
   #define SQLITE_UTF16    2
   #define SQLITE_UTF16BE  3
   #define SQLITE_UTF16LE  4
   #define SQLITE_ANY      5
nArg 参数用来表明自定义函数的参数个数. 如果参数值为0，则表示接受任意个数的参数. 用 eTextRep 参数来表明传入参数的编码形式. 参数值可以是上面的五种预定义值. SQLite3 允许同一个自定义函数有多种不同的编码参数的版本. 数据库引擎会自动选择转换参数编码个数最少的版本使用.
普通的函数只需要设置 xFunc 参数，而把 xStep 和 xFinal 设为NULL. 聚合函数则需要设置 xStep 和 xFinal 参数，然后把 xFunc 设为NULL. 该方法和使用sqlite3_create_aggregate() API一样.
sqlite3_create_function16()和sqlite_create_function()的不同就在于自定义的函数名一个要求是 UTF-16 编码，而另一个则要求是 UTF-8.
请注意自定函数的参数目前使用了sqlite3_value结构体指针替代了SQLite version 2.X中的字符串指针. 下面的函数用来从sqlite3_value结构体中提取数据:
   const void *sqlite3_value_blob(sqlite3_value*);
   int sqlite3_value_bytes(sqlite3_value*);
   int sqlite3_value_bytes16(sqlite3_value*);
   double sqlite3_value_double(sqlite3_value*);
   int sqlite3_value_int(sqlite3_value*);
   long long int sqlite3_value_int64(sqlite3_value*);
   const unsigned char *sqlite3_value_text(sqlite3_value*);
   const void *sqlite3_value_text16(sqlite3_value*);
   int sqlite3_value_type(sqlite3_value*);
上面的函数调用以下的API来获得上下文内容和返回结果:
   void *sqlite3_aggregate_context(sqlite3_context*, int nbyte);
   void *sqlite3_user_data(sqlite3_context*);
   void sqlite3_result_blob(sqlite3_context*, const void*, int n, void(*)(void*));
   void qlite3_result_double(sqlite3_context*, double);
   void sqlite3_result_error(sqlite3_context*, const char*, int);
   void sqlite3_result_error16(sqlite3_context*, const void*, int);
   void sqlite3_result_int(sqlite3_context*, int);
   void sqlite3_result_int64(sqlite3_context*, long long int);
   void sqlite3_result_null(sqlite3_context*);
   void sqlite3_result_text(sqlite3_context*, const char*, int n, void(*)(void*));
  void sqlite3_result_text16(sqlite3_context*, const void*, int n, void(*)(void*));
   void sqlite3_result_value(sqlite3_context*, sqlite3_value*);
   void *sqlite3_get_auxdata(sqlite3_context*, int);
   void sqlite3_set_auxdata(sqlite3_context*, int, void*, void (*)(void*));
（4）用户自定义排序规则
下面的函数用来实现用户自定义的排序规则:
sqlite3_create_collation(sqlite3*, const char *zName, int eTextRep, void*,
int(*xCompare)(void*,int,const void*,int,const void*));
sqlite3_create_collation16(sqlite3*, const void *zName, int eTextRep, void*,
int(*xCompare)(void*,int,const void*,int,const void*));
sqlite3_collation_needed(sqlite3*, void*,
void(*)(void*,sqlite3*,int eTextRep,const char*));
sqlite3_collation_needed16(sqlite3*, void*,
void(*)(void*,sqlite3*,int eTextRep,const void*));
sqlite3_create_collation() 函数用来声明一个排序序列和实现它的比较函数. 比较函数只能用来做文本的比较. eTextRep 参数可以取如下的预定义值 SQLITE_UTF8, SQLITE_UTF16LE, SQLITE_UTF16BE, SQLITE_ANY，用来表示比较函数所处理的文本的编码方式. 同一个自定义的排序规则的同一个比较函数可以有 UTF-8, UTF-16LE 和 UTF-16BE 等多个编码的版本. sqlite3_create_collation16()和sqlite3_create_collation() 的区别也仅仅在于排序名称的编码是 UTF-16 还是 UTF-8.
可以使用 sqlite3_collation_needed() 函数来注册一个回调函数，当数据库引擎遇到未知的排序规则时会自动调用该函数. 在回调函数中可以查找一个相似的比较函数，并激活相应的sqlite_3_create_collation()函数. 回调函数的第四个参数是排序规则的名称，同样sqlite3_collation_needed采用 UTF-8 编码. sqlite3_collation_need16() 采用 UTF-16 编码.