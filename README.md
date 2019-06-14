# 2019-spring-gt

## 如何執行

### Linux

```
# clone first (with submodule)
$ git clone --recursive https://github.com/CHIH-CHAN/Graph-Theory-Project-Template.git
# build
$ make 
# run 
$ ./main.out
```
### Input file
輸入檔名為"topo.txt"，須放在目錄底下 \
輸入格式範例如下:\
a b\
b c \
則此檔中有a b c三個vertex，且ａ<->b b<->c

#### Default Input file (Example)
```
a b
b c
c d
d e
e f
f g
g h
h i
i j
b d
c e
f h
g i
```
### Output file
程式執行後，會自動產生以下檔案並放在目錄底下 
1. output.txt\
![Alt text](/pic/output.PNG)
2. plot.dot
![Alt text](/pic/plot.PNG)


## 程式功能：
解決中國郵差問題，並將路徑輸出到"output.txt"。

## 程式限制：
只適用於無向圖、端點最多只能10個、端點之間的連線最多一條。

## 解題邏輯：
首先先使用Fake-mininet的API將輸入檔案讀進來轉換成拓樸並使用Gplot繪圖，然後將其轉換成adjacency matrix。\
計算各個點的連接後找出其degree為奇數的點，利用Single-Pair Shortest Path來找到配對後最少的重複路徑，\
再將這些路徑加入adjacency matrix之中，使此圖成為一尤拉圖，\
最後利用Depth-First Search來進行尋找路徑的動作，並將這個路徑輸出到output.txt。

## 參考連結：
https://www.geeksforgeeks.org/dijkstras-shortest-path-with-minimum-edges/ \
http://alrightchiu.github.io/SecondRound/shortest-pathintrojian-jie.html \
http://alrightchiu.github.io/SecondRound/graph-depth-first-searchdfsshen-du-you-xian-sou-xun.html
