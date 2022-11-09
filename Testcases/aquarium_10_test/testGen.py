import random
import os
def makeNewTest(n, p):
  grid = [[-1 for i in range(n+2)] for j in range(n+2)]
  dx = [1,0,-1,0]
  dy = [0,-1,0,1]
  top = [] # 맨위
  bottom = [] # 맨아래
  maxn = -1
  for i in range(1,1+n):
      for j in range(1,1+n):
          s = [grid[i+dx[k]][j+dy[k]] for k in range(4)]
          if -1 not in s:
              grid[i][j] = s[random.randint(0,3)]
              if random.random()>p:
                  grid[i][j] = maxn+1
                  maxn += 1
                  top.append(i)
                  bottom.append(i)
              bottom[grid[i][j]] = i
          else:
              ss=[k for k in s if k!=-1]
              if len(ss)>0:
                  grid[i][j] = ss[random.randint(0,len(ss)-1)]
                  if random.random()>p:
                      grid[i][j] = maxn+1
                      maxn += 1
                      top.append(i)
                      bottom.append(i)
                  bottom[grid[i][j]] = i
              else:
                  grid[i][j] = maxn+1
                  maxn+=1
                  top.append(i)
                  bottom.append(i)
  # for i in grid[1:-1]:print(*i[1:-1])
  # print(top)
  # print(bottom)
  # print()
  grid2 = [[0 for i in range(n)] for j in range(n)]
  height = []
  for i in range(maxn+1):
      height.append(random.randint(0,bottom[i]-top[i]+1))
  # print(height)
  for i in range(n):
      for j in range(n):
          if i+1>top[grid[i+1][j+1]]+bottom[grid[i+1][j+1]]-top[grid[i+1][j+1]]-height[grid[i+1][j+1]]:
              grid2[i][j] = 1
  resstr = ""
  resstr += f"{n} {n}\n"
  for i in range(n):
    resstr += str(sum(grid2[i]))
    resstr += " "
  for k in range(n):
    resstr += str(sum([grid2[i][k] for i in range(n)]))
    resstr += " "
  resstr += '\n'
  for i in grid[1:-1]:
    for k in i[1:-1]:
      resstr += str(k+1)
      resstr += ' '
    resstr += '\n'
  return resstr
for r in range(0, 100, 5):
  if not os.path.exists(f'aquarium_5_{r}_test'):
    os.makedirs(f'aquarium_5_{r}_test')
  for i in range(100):
    f = open(f'aquarium_5_{r}_test/test{i}.txt', 'w')
    f.write(makeNewTest(5, r/100))
    f.close()
