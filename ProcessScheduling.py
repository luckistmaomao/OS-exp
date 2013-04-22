#encoding=utf8
import copy


def display(l):
    for i in l:
        print i[0],"响应时间",i[-2],"周转时间",i[-1]



file = open("JOB1.txt","r")

lines = file.readlines()

file.seek(0)
print file.read()

num = len(lines)

q = [[] for i in range(num)]

for i in range(num):
    line = lines[i].strip('\n')
    a = []
    a = line.split(",")
    for j in range(len(a)):
        if j==0:
            q[i].append(a[j])
        else:
            q[i].append(int(a[j]))



#短作业优先
p = copy.deepcopy(q) 

for i in p:
    i.append(0)
print "短作业优先算法"
time = 0
num = len(p)
for j in range(num) :
    index = -1
    min = 10000
    for i in range(num):
        if p[i][1]<=time and p[i][4]==0:
            if min>p[i][2]:
                index = i
                min =p[i][2]
    p[index][4]=1
    p[index].append(time-p[index][1])
    time = time + p[index][2]
    p[index].append(time-p[index][1])
    print p[index][0],time,
    if j<=3:
        print "->",
    else:
        print " end"

display(p)


#优先数算法
print "优先数算法"
p = copy.deepcopy(q) 

for i in p:
    i.append(0)
time = 0
num = len(p)
for j in range(num) :
    index = -1
    min = 10000
    for i in range(num):
        if p[i][1]<=time and p[i][4]==0:
            if min>p[i][3]:
                index = i
                min =p[i][3]
    p[index][4]=1
    p[index].append(time-p[index][1])   #响应时间
    time = time + p[index][2]
    p[index].append(time-p[index][1])   #周转时间
    print p[index][0],time,
    if j<=3:
        print "->",
    else:
        print " end"


display(p)


#时间片轮转算法
for slice in range(1,4):  #时间片分别为1,2,3
    print "时间片为",slice
    count = 0
    p = copy.deepcopy(q)
    for i in p:
        i.append(0)
        i.append(0)
    time = 0
    num = len(p)
    while count < num:
        for j in range(num):
            if p[j][1]<=time and p[j][4]==0:
                remaining = p[j][2] - p[j][5]
                if p[j][5]==0:    
                    p[j].append(time-p[j][1])
                if remaining > slice:
                    p[j][5] += slice
                    time = time + slice
                else:
                    p[j][5] += remaining
                    time += remaining
                if p[j][2]==p[j][5]:
                    p[j][4] = 1
                    count += 1
                    p[j].append(time-p[j][1])
                print p[j][0],time
    display(p)                

