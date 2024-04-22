# MCC Task Scheduler Algorithm

This script implements a sophisticated task scheduling algorithm designed for multi-core computing environments with cloud integration. It aims to optimize the execution of task graphs by distributing tasks across local cores and cloud resources based on execution times and dependencies.

## Scientific Foundation

The algorithm implemented in this script is based on the research paper:

**"Energy and Performance-Aware Task Scheduling in a Mobile Cloud Computing Environment"**
by Xue Lin, Yanzhi Wang, Qing Xie, and Massoud Pedram, from the Department of Electrical Engineering at the University of Southern California.

### Paper Overview

This paper addresses the optimization of task scheduling in mobile cloud computing (MCC) environments. It focuses on minimizing energy consumption while adhering to task precedence and completion time constraints. The paper introduces a novel scheduling algorithm that involves:
1. Identifying tasks to offload to the cloud.
2. Mapping remaining tasks onto local or heterogeneous cores.
3. Scheduling all tasks efficiently to meet predefined constraints.

### Key Contributions

- The proposed algorithm begins with a minimal-delay scheduling solution and optimizes energy consumption by intelligently migrating tasks between local cores and the cloud.
- A linear-time rescheduling algorithm is proposed for efficient task migration, aiming to maintain or improve system performance and energy efficiency.
- Simulation results in the paper demonstrate that the algorithm can achieve significant energy reductions compared with traditional methods.

### Implementation in the Script

The implementation in this script follows the algorithm's methodology for task scheduling by:
- Dynamically assigning tasks to either local processing units or the cloud based on execution time estimates and energy considerations.
- Utilizing a priority-based scheduling approach to ensure that all tasks are executed within their required time frames while minimizing overall energy consumption.

This paper forms the scientific basis of the algorithm implemented in the script, guiding the principles behind task distribution and energy efficiency in a simulated MCC environment.

## Features

- **Dynamic Task Scheduling**: Distributes tasks dynamically across local cores and a cloud environment.
- **Supports Multiple Task Graphs**: Compatible with different task graphs, each representing unique task dependencies and requirements.
- **Custom Execution Profiles**: Utilizes execution time tables tailored for specific task graphs to ensure accurate scheduling.
- **Performance Metrics**: Outputs detailed metrics including task start and finish times, core utilization, and cloud interaction.

## Requirements

- A system with support for C++ compilation (e.g., GCC compiler).
- Basic libraries included with standard C++ installations.

## Installation

1. Ensure you have a C++ compiler installed on your system.
2. Download the script `mcc_task_scheduler.cpp` to your local machine.
3. Compile the script using a C++ compiler:
   ```bash
   g++ -o mcc_task_scheduler mcc_task_scheduler.cpp
   ```
4. Run the compiled program:
  ```bash
  ./mcc_task_scheduler
  ```

## Usage

- **Modifying Task Graphs and Execution Tables**: Adjust the predefined task graphs and execution tables within the script to reflect the specific requirements and topology of the tasks you intend to schedule.
- **Execution**: Once compiled, run the executable as described in the Installation section.
- **Monitoring Results**: The program outputs scheduling details such as task assignments, core utilizations, and cloud interactions along with performance metrics.

## Configuration Parameters

- **TASK_NUMBER**: Defines the number of tasks within the graph. Default is set to 10 but can be adjusted as needed.
- **Execution Time Tables**: Modify the execution times for each task within the tables to match the characteristics of your specific tasks.

## Output

- Visual representations of task assignments across cores and cloud.
- Timelines for each task showing start and end times.
- Performance metrics including total execution time and energy consumption.
- Detailed logs for operations, particularly when tasks are shifted between cores and cloud to optimize performance.

## Troubleshooting

- **Compilation Errors**: Ensure your compiler is up-to-date and compatible with C++ standards used in the script.
- **Runtime Errors**: Check the integrity and appropriateness of the task graphs and execution tables. Ensure that all indices and array sizes are correctly configured.
- **Performance Issues**: Adjust task execution times, tweak scheduling parameters, or refactor task dependencies within the graph for better optimization.
