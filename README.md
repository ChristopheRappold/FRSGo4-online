# FRS-Go4 Online analysis

The new online analysis for data monitoring, quality check and online visualization of the data stream of the FRS experiments at GSI. It is based on the go4 framework developed at GSI [1] that is used as a GUI framework and multi-thread data analysis.

The analysis code has been cleaning up and reorganized for more flexibility. Experimental parameters have been gathered and isolated into separated scripts located in the _setup_ directory. They are selected through the configuration file _config.txt_ with the config option "NameExperiment". The analysis code isolates properly event structures and event processing tasks. The processing tasks are successively _Unpack_, _Sort_, _Calib_, _Analysis_ and _User_. The tasks and event structures up-to Analysis step are defined and set automatically from the setup scripts. Calibration parameters, mapping from electronics channels to detector data structures and physical observables are already handled: new configurations are set through a new setup script that needs to be added in the setup directory and committed in the git repository. For a new experiment, a new git branch has to be created and commits specific to this setup/experiment have to be submitted to the git repository at GSI.


[1] https://www.gsi.de/en/work/research/experiment_electronics/data_processing/data_analysis/the_go4_home_page.htm

## How to install 

  + What is mandatory:
    - minimum C++ 11 compiler
	- ROOT v6 (> 6.08)
	- Go4 v5 (> 5.0.1)
     
  + Go to src/ and make:
```sh
	cd src && make && cd ..
```

## Organization

The directory organization:

| Dir:   | Usage:                                                                                           |
|--------|--------------------------------------------------------------------------------------------------|
| map/   | Gathers mapping files of the crates and electronics channels                                     | 
| src/   | Source directory                                                                                 |
| setup/ | Gathers the setup scripts that are selected and injected into the initialization of the analysis |
| lml/   | Gathers the lml files for listing lmd input files                                                |
| data/  | Link to the data storage directory                                                               |


 + config.txt : Load the tag of name of experiment "NameExperiment" that is associated with the setup script that should be present in the setup directory. Example:
 ```sh
 cat config.txt
 
 # The name of the experiment has to set here
NameExperiment s411_jun16

 ```
The file "setup/setup\_s411\_jun16.C" will be loading in the analysis procedure.

## Usage

Via the go4 interface :
```sh
	go4 
```
or for offline processing without the associated GUI:
```sh
	go4analysis -lib ./libGo4UserAnalysis.so -file "input_file" -disable-asf -step "NameStep" -enable-step -enable-store -store "output_file" 
```
Please refer to the go4analysis help for more precise information. 



## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b feature/fooBar`)
3. Commit your changes (`git commit -am 'Add some fooBar'`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Create a new Merge Request

  
  
