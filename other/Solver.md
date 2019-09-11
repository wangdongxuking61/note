1. `性能对比`
```
http://www.gurobi.com/resources/switching-to-gurobi/open-source-solvers
http://plato.asu.edu/bench.html
```
2. `lp文件格式`
```
\ format: http://docs.mosek.com/7.1/javaapi/The_LP_file_format.html
\ milo1.lp
maximize 
obj: x1 + 6.4e-01 x2 
subject to 
c1:  5e+01 x1 + 3.1e+01 x2 <= 2.5e+02 
c2:  3e+00 x1 - 2e+00 x2 >= -4e+00 
bounds 
 0 <= x1 <= +infinity 
 0 <= x2 <= +infinity 
general 
 x1 x2
binary
end 
```
3. `gurobi`
```
# http://www.gurobi.com/resources/examples/example-models-overview
gurobi_cl coins.lp
gurobi_cl ResultFile=coins.sol coins.lp
OutputFlag(LogToConsole+LogFile)
```
4. `cplex`
```
cd /Users/xuxu/Applications/IBM/ILOG/CPLEX_Studio1263/cplex/bin/x86-64_osx
./cplex -c "read demo.lp" "opt" "disp sol var -" "quit"
```
5. `Anaconda2`
On “Installation Type” you may choose to install in another location. Standard install will put Anaconda in your home user directory .