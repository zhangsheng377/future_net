#实现失败的算法：
###1.节点出度全部遍历完之后，所确定的代价和路径，在再有节点访问到该节点时，其实并不能使用这个代价和路径，因为这样会使某些V'中的节点访问不到。
###2.用多线程的方式来搜索，最后会由于规模太大，开的线程太多，从而把内存撑爆！  
  
#准备实现的：
###1.全面向对象式的思维方式，干脆就把每个node写成一个路由器，先用迪杰斯卡尔之类的算出600^3规模的全局路由表，每个路由根据优先未到达的V'中的目的进行转发，根据传来的路径判断是否已访问过。这样可能的转发接口或许会比较少，总体转发的规模肯定比深度搜索要小。因为转发的目的是优先V'，所以可能会有向几个目的转发都从同一个接口走的情况。  
  
#已经实现的算法：
###1.利用最小值剪枝：当算出一个可行路径的时候，保存其代价。当深度搜索到其他节点时，若此时代价已大于最小可行路径的代价，则该节点就不用再继续算了。
###2.利用V'中肯定不包含SourceID和DestinationID，减少判定时for的长度。
###3.最小代价剪枝：先将node里的出度link按照在V'里的在前面、cost小的在前面的准则排序，这样很快就能有一个可行解了。
###4.修改了link的排序的比较函数，把目的节点放在了V'的后面第一个。
###5.把pathTemp.nodesID给copy了一个副本，排完序之后，把V'里的每一个用二分在副本里搜索。比之前的两个for查找要快很多(应该吧～～)
###6.设置了快到10s时立即返回。
  
  ————————————————————————————————————————————————————  
HUAWEI Code Craft 2016 “未来网络·寻路”初赛题目介绍 文档密级：公开  
2016-3-3 华为保密信息,未经授权禁止扩散 
HUAWEI Code Craft 2016  
未来网络 ·寻路  
初赛 题目介绍  
2016.03.04  

前言  
赛题源自“未来网络”业务发放中的路由计算问题。算路问题属于基础算法问题，在图论、网络、交通等各个方面均有着广泛的研究与运用，里面不乏一些经典的算法，例如最短路中的广度优先搜索，Dijkstra算法等。网络算路问题的更优算法实现对于网络资源高效配置具有重要价值。  

1 问题定义  
给定一个带权重的有向图G=(V,E)，V为顶点集，E为有向边集，每一条有向边均有一个权重。对于给定的顶点s、t，以及V的子集V'，寻找从s到t的不成环有向路径P，使得P经过V'中所有的顶点(对经过V'中节点的顺序不做要求)。  
若不存在这样的有向路径P，则输出无解，程序运行时间越短，则视为结果越优；若存在这样的有向路径P，则输出所得到的路径，路径的权重越小，则视为结果越优，在输出路径权重一样的前提下，程序运行时间越短，则视为结果越优。  
说明：  
1）图中所有权重均为[1，20]内的整数；  
2）任一有向边的起点不等于终点；  
3）连接顶点A至顶点B的有向边可能超过一条，其权重可能一样，也可能不一样；  
4）该有向图的顶点不会超过600个，每个顶点出度(以该点为起点的有向边的数量)不超过8；  
5）V'中元素个数不超过50；  
6）从s到t的不成环有向路径P是指，P为由一系列有向边组成的从s至t的有向连通路径，且不允许重复经过任一节点；  
7）路径的权重是指所有组成该路径的所有有向边的权重之和。  

2 输入与输出  
输入文件格式  
以两个.csv文件(csv是以逗号为分隔符的文本文件)给出输入数据，一个为图的数据(G)，一个为需要计算的路径信息(s,t,V')。文件每行以换行符(ASCII'\n'即0x0a)为结尾。  
1）图的数据中，每一行包含如下的信息：  
LinkID,SourceID,DestinationID,Cost  
其中，LinkID为该有向边的索引，SourceID为该有向边的起始顶点的索引，DestinationID为该有向边的终止顶点的索引，Cost为该有向边的权重。顶点与有向边的索引均从0开始编号(不一定连续，但用例保证索引不重复)。  
2）路径信息中，只有一行如下数据：  
SourceID,DestinationID,IncludingSet  
其中，SourceID为该路径的起点，DestinationID为该路径的终点，IncludingSet表示必须经过的顶点集合V'，其中不同的顶点索引之间用'|'分割。  

输出文件格式  
输出文件同样为一个.csv文件。  
1）如果该测试用例存在满足要求的有向路径P，则按P经过的有向边顺序，依次输出有向边的索引，索引之间用'|'分割；  
2）如果该测试用例不存在满足要求的有向路径P，则输出两个字符NA；  
3）只允许输出最多一条有向路径。  

3 单个用例的评分机制  
有解用例的排名机制  
按下面流程对参赛者结果进行排名：  
Step1：对于提交的结果，进行合法性检验(详见题目描述)；  
Step2：程序运行时间不得超过10s；  
若不满足上述的结果则本用例得分为0；  
Step3：计算提交的路径的权重，权重越小，排名越优；  
Step4：在权重相同的结果里，用程序运行时间进行排名，时间越短，排名越优。  

无解用例的排名机制
按下列流程对参赛者结果进行排名：  
Step1：对于提交的结果，验证是否识别出该用例无解，若无法识别或者算法运行时间超10s，则本用例得分为0；  
Step2：用程序的运行时间进行排名，时间越短，排名越优。  

单个用例的评分标准如下：  
根据上面排名流程得到的排名，使用标准分计分(排名第一的提交者为100分)。  
若所有人均未得到正确结果，则所有人均得分为0。  

4 最终得分机制  
平台会使用N个测试用例判题，该N个测试用例分为初级、中级、高级三个等级，参赛者对于每个测试用例都会得到一个百分制分数，使用加权平均分(初级权重为0.2，中级权重为0.3，高级权重为0.5)作为该参赛者的最终得分。   特别说明：在比赛初期，平台只放出初级、中级的测试用例，故此时满分为50分，在比赛后期，才会放出高级测试用例（具体发放时间会在网站公告通知），此时满分才为100分，请各位参赛者注意。  

5 简单用例说明  
在如上图所示的有向图中，我们会得到下面的有向图信息：  
0,0,1,1  
1,0,2,2  
2,0,3,1  
3,2,1,3  
4,3,1,1  
5,2,3,1  
6,3,2,1  
如果此时需要寻找从0到1的路径，且必须经过顶点2和3，我们会得到如下的路径信息：  
0,1,2|3  
对于该用例，可以找到如下两条可行路径:  
1|5|4  
2|6|3  
由于第一条路径的权重为4，第二条路径的权重为5，所以此时最优解应该1|5|4。  
