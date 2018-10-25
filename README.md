# `scheduler` Tasks Scheduler Simulation

(Linux) Instructions to download, build and run the Tasks Scheduler Simulation.



## 1. Download project from GitHub
Open a terminal window and enter:
```bash

$ git clone git@github.com:Aightech/scheduler.git
```
## 2. COMPILE
Navigate to the ```scheduler``` root directory and execute
```bash

$ make
```

## 3. Execute
In the file directory, enter:
```bash

$ ./scheduler <scheduler mode>
```
with ```<mode of scheduler>```:
* 0 = defferable server
* 1 = EDF
* 2 = Static Priority
* 3 = Round Robin

## 4. RQ
The preemptivity is not 'real' because the scheduler is called at each step. So it work as if (indeed if nothing from the last ste the scheduler wil choose the same task) but it is not.
