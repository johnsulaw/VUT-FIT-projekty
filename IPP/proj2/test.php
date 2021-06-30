<?php

class TestResults{
    public $test_name;
    public $test_rc;
    public $test_out;
    public $test_descr;

    public function __construct($name, $rc, $out, $description){
        $this->test_name = $name;
        $this->test_rc = $rc;
        if($out == null){
            $this->test_out = "None\n";
        }else{
            $this->test_out = $out;
        }
        $this->test_descr = $description;
    }
}

class Options {
    public $recursive;
    public $int_only;
    public $parse_only;
    public $directory;
    public $parse_script;
    public $int_script;
    public $jexamxml;

    public function __construct()
    {
        $this->recursive=false;
        $this->int_only=false;
        $this->parse_only=false;
        $this->directory=getcwd();
        $this->parse_script='./parse.php';
        $this->int_script='./interpret.py';
        $this->jexamxml='/pub/courses/ipp/jexamxml/jexamxml.jar';
    }
}


function create_tmp_file(){
    if(($temp_file = fopen("temp.out", "w")) == FALSE){
        fwrite(STDERR, "Failed to open temporary file!\n");
        exit(12);
    }else return $temp_file;
}

function delete_tmp_file(){
    unlink("temp.out");
}

function run_test($Options, $mode, $recursive){
    $results_array = [];

    foreach($recursive as $path){
        $test_files = scandir($path);
        $source_files = [];

        //get all .src files in test folder
        foreach ($test_files as $source_file){
            if(preg_match('/.*\.src/', $source_file)){
                array_push($source_files, $source_file);
            }
        }

        //get or generate .in .out and .rc files for one test
        foreach ($source_files as $source_file){
            $test_name = explode(".", $source_file)[0];

            if(!file_exists($Options->directory.$test_name.".in")){
                if(($in_file = fopen($Options->directory.$test_name.".in", "w")) == FALSE){
                    fwrite(STDERR, "Failed to create ".$test_name.".in file!\n");
                    exit(11);
                }
            }
            if(!file_exists($Options->directory.$test_name.".out")){
                if(($out_file = fopen($Options->directory.$test_name.".out", "w")) == FALSE){
                    fwrite(STDERR, "Failed to create ".$test_name.".out file!\n");
                    exit(11);
                }
            }
            if(!file_exists($Options->directory.$test_name.".rc")){
                if(($rc_file = fopen($Options->directory.$test_name.".rc", "w")) == FALSE){
                    fwrite(STDERR, "Failed to create ".$test_name.".rc file!\n");
                    exit(11);
                }
                fwrite($rc_file, "0");
            }

            //create temp file to which output of scripts will be stored
            $temp_file = create_tmp_file();

            //if user put in "int-only" argument
            if($mode == "int"){
                unset($int_out);
                exec('python3.6 ' .$Options->int_script .' --source=' .$Options->directory.$source_file .' < ' .$Options->directory.$test_name.".in", $int_out, $int_rc);
                fwrite($temp_file, implode("\n", $int_out));
                if(count($int_out) > 0){
                    fwrite($temp_file, "\n");
                }

                if($int_rc == file_get_contents($Options->directory.$test_name.".rc")){
                    if($int_rc == 0){
                        exec('diff ' ."temp.out " .$Options->directory.$test_name.".out " ."-w", $disregard, $diff_rc);
                        if($diff_rc == 0){
                            $test_result = new TestResults($test_name, $int_rc, file_get_contents("temp.out"), "OK");
                            array_push($results_array, $test_result);
                        }else{
                            $description = "Generated output does not match expected output in ".$test_name.".out\n"
                                ."\t\tEXPECTED OUTPUT: ".file_get_contents($Options->directory.$test_name.".out")
                                ."\t\tGENERATED OUTPUT: ".file_get_contents("temp.out");
                            $test_result = new TestResults($test_name, $int_rc, file_get_contents("temp.out"), $description);
                            array_push($results_array,$test_result);
                        }
                    }else{
                        $test_result = new TestResults($test_name, $int_rc, file_get_contents("temp.out"), "OK");
                        array_push($results_array, $test_result);
                    }
                }else{
                    $description = "Generated return code does not match expected return code in ".$test_name.".rc\n"
                        ."\t\tEXPECTED RETURN CODE: ".file_get_contents($Options->directory.$test_name.".rc")
                        ."\t\tGENERATED RETURN CODE: ".$int_rc;
                    $test_result = new TestResults($test_name, $int_rc, file_get_contents("temp.out"), $description);
                    array_push($results_array,$test_result);
                }
                //if user put in "parse-only" argument
            }elseif($mode == "parse"){
                unset($parse_out);
                exec('php7.4 ' .$Options->parse_script .' < ' .$Options->directory.$source_file, $parse_out, $parse_rc);
                fwrite($temp_file, implode("\n", $parse_out));
                if(count($parse_out) > 0){
                    fwrite($temp_file, "\n");
                }

                //compare output and expected output using JExamXML
                if($parse_rc == 0){
                    exec('java -jar ' .$Options->jexamxml .' ' .$Options->directory.$test_name.".out " ."temp.out", $output, $compare_rc);
                    if($compare_rc == 0){
                        $test_result = new TestResults($test_name, $parse_rc, null, "OK");
                        array_push($results_array, $test_result);
                    }else{
                        $description = "Generated output does not match expected output in ".$test_name.".out";
                        $test_result = new TestResults($test_name, $parse_rc, null, $description);
                        array_push($results_array, $test_result);
                    }
                }else{
                    if($parse_rc == file_get_contents($Options->directory.$test_name.".rc")){
                        $test_result = new TestResults($test_name, $parse_rc, null, "OK");
                        array_push($results_array, $test_result);
                    }else{
                        $description = "Generated return code does not match expected return code in ".$test_name.".rc\n"
                            ."\t\tEXPECTED RETURN CODE: ".file_get_contents($Options->directory.$test_name.".rc")
                            ."\t\tGENERATED RETURN CODE: ".$parse_rc;
                        $test_result = new TestResults($test_name, $parse_rc, null, $description);
                        array_push($results_array, $test_result);
                    }
                }

            }elseif($mode == "both"){
                unset($parse_out);
                exec('php7.4 ' .$Options->parse_script .' < ' .$Options->directory.$source_file, $parse_out, $parse_rc);
                if($parse_rc == 0){
                    // write XML representation to temporary file
                    fwrite($temp_file, implode("\n", $parse_out));
                    if(count($parse_out) > 0){
                        fwrite($temp_file, "\n");
                    }

                    unset($int_out);
                    exec('python3.6 ' .$Options->int_script .' --source=temp.out/' .' < ' .$Options->directory.$test_name.".in " ."-w", $int_out, $int_rc);
                    delete_tmp_file();
                    $temp_file = create_tmp_file();
                    // empty the temporary file and write output to it
                    fwrite($temp_file, implode("\n", $int_out));
                    if(count($int_out) > 0){
                        fwrite($temp_file, "\n");
                    }

                    if($int_rc == file_get_contents($Options->directory.$test_name.".rc")){
                        if($int_rc == 0){
                            exec('diff ' ."temp.out " .$Options->directory.$test_name.".out", $disregard, $diff_rc);
                            if($diff_rc == 0){
                                $test_result = new TestResults($test_name, $int_rc, file_get_contents("temp.out"), "OK");
                                array_push($results_array, $test_result);
                            }else{
                                $description = "Generated output does not match expected output in ".$test_name.".out\n"
                                    ."\t\tEXPECTED OUTPUT: ".file_get_contents($Options->directory.$test_name.".out");
                                $test_result = new TestResults($test_name, $int_rc, file_get_contents("temp.out"), $description);
                                array_push($results_array,$test_result);
                            }
                        }else{
                            $test_result = new TestResults($test_name, $int_rc, file_get_contents("temp.out"), "OK");
                            array_push($results_array, $test_result);
                        }
                    }else{
                        $description = "Generated return code does not match expected return code in ".$test_name.".rc\n"
                            ."\t\tEXPECTED RETURN CODE: ".file_get_contents($Options->directory.$test_name.".rc")
                            ."\t\tGENERATED RETURN CODE: ".$int_rc;
                        $test_result = new TestResults($test_name, $int_rc, file_get_contents("temp.out"), $description);
                        array_push($results_array,$test_result);
                    }
                }else{
                    if($parse_rc == file_get_contents($Options->directory.$test_name.".rc")){
                        $test_result = new TestResults($test_name, $parse_rc, null, "OK");
                        array_push($results_array, $test_result);
                    }else{
                        $description = "Generated return code does not match expected return code in ".$test_name.".rc\n"
                            ."\t\tEXPECTED RETURN CODE: ".file_get_contents($Options->directory.$test_name.".rc")
                            ."\t\tGENERATED RETURN CODE: ".$parse_rc;
                        $test_result = new TestResults($test_name, $parse_rc, null, $description);
                        array_push($results_array, $test_result);
                    }
                }
            }

            delete_tmp_file();
        }
    }
    return $results_array;
}

function evaluate_results($results_array){
    $total = 0;
    $bad = 0;
    $ok = 0;

    foreach ($results_array as $result) {
        $total++;
        if (strcmp($result->test_descr, "OK") == 0) {
            $ok++;
        } else {
            $bad++;
        }
    }

    $stats = [];
    array_push($stats, $total, $bad, $ok);

    return $stats;
}

function recursive_get_paths($dir, &$paths_array = array()){
    $files = scandir($dir);

    foreach($files as $key => $value){
        $path = realpath($dir . DIRECTORY_SEPARATOR . $value);
        if(!is_dir($path)){
            $paths_array[] = $path;
        }elseif($value != "." && $value != ".."){
            recursive_get_paths($path, $paths_array);
            $paths_array[] = $path;
        }
    }
    return $paths_array;
}

function create_html($stats, $results){
    $total = $stats[0];
    $bad = $stats[1];
    $ok = $stats[2];

    $rows_code = generate_table_rows($results);

    $html_code =
        '<!doctype html>
        <html lang=\"cz\"
        <head>
            <title>IPPcode20 test.php</title>
            <style>
                b {
                color: orangered;
                }
                body{
                background-color: lightgoldenrodyellow;
                }
                table, th {
                border: 1px solid black;
                }
            </style>
        </head>
        <body>
           <h1>IPPcode20 TEST RESULTS</h1>
           <b>TOTAL TESTS: </b>'.$total.
           '<br/><b>PASSED TESTS: </b>'.$ok.
           '<br/><b>FAILED TESTS: </b>'.$bad.
           '<br/><br/>Test evalueates as OK, if generated return code matches expected return code. 
            <table style="width:100%">'
                .$rows_code.
           '</table>
           </body>';

    if(($html_file = fopen("results.html", "w")) == FALSE){
        fwrite(STDERR, "Failed to create results.html file!\n");
        exit(12);
    }
    fwrite($html_file, $html_code);
    fclose($html_file);
}

function generate_table_rows($results){
    $rows_code = "<tr style='background-color: white'>
                    <th>TEST NAME</th>
                    <th>RETURN CODE</th>
                    <th>OUTPUT/ERROR MESSAGE</th>
                    <th>DESCRIPTION</th>
                  </tr>";

    foreach($results as $result){
        if(strcmp($result->test_descr, "OK") == 0){
            $row = "<tr style='background-color: green'>
                    <th>".$result->test_name."</th>
                    <th>".$result->test_rc."</th>
                    <th>".$result->test_out."</th>
                    <th>".$result->test_descr."</th>
                </tr>";
            $rows_code = $rows_code.$row;
        }else{
            $row = "<tr style='background-color: red'>
                    <th>".$result->test_name."</th>
                    <th>".$result->test_rc."</th>
                    <th>".$result->test_out."</th>
                    <th>".$result->test_descr."</th>
                </tr>";
            $rows_code = $rows_code.$row;
        }
    }

    return $rows_code;
}

function check_arguments($argc, $argv, $Options){
    // default values
    $parse_only_arg = false;
    $int_only_arg = false;
    $parse_script_arg = false;
    $int_script_arg = false;

    // script can be run with no arguments or 6 arguments max
    if($argc < 1 or $argc > 6) {
        echo("Invalid number of arguments for test.php!\n");
        exit(100);
    }

    // iterating over arguments and setting values
    foreach($argv as $argument){
        if(preg_match('/--help/', $argument)){
            print("Toto je placebo help argument.\n");
            exit(0);
        }
        //DIRECTORY ARG
        elseif(preg_match('/--directory=.*/', $argument)) {
            $directory_arg = true;
            $Options->directory = explode("=", $argument)[1];
            // if last character of path is not directory separator, add it
            if(strcmp(substr($Options->directory, -1), "/") != 0){
                $Options->directory = $Options->directory.DIRECTORY_SEPARATOR;
            }
        }
        // RECURSIVE ARG
        elseif(preg_match('/--recursive/', $argument)){
            $Options->recursive = true;
        }
        // PARSE-SCRIPT ARG
        elseif(preg_match('/--parse-script=.*/', $argument)){
            $parse_script_arg = true;
            $Options->parse_script = explode("=", $argument)[1];
        }
        // INT-SCRIPT ARG
        elseif(preg_match('/--int-script=.*/', $argument)){
            $int_script_arg = true;
            $Options->int_script = explode("=", $argument)[1];
        }
        // PARSE-ONLY ARG
        elseif(preg_match('/--parse-only/', $argument)){
            $parse_only_arg = true;
            $Options->parse_only = true;
        }
        // INT-ONLY ARG
        elseif(preg_match('/--int-only/', $argument)){
            $int_only_arg = true;
            $Options->int_only = true;
        }
        // JEXAMXML ARG
        elseif(preg_match('/--jexamxml=.*/', $argument)){
            $jexamxml_arg = true;
            $Options->jexamxml = explode("=", $argument)[1];
        }
    }

    if($parse_only_arg == true){
        if(($int_only_arg == true) or ($int_script_arg == true)){
            print("Unallowed combination of arguments!\n");
            exit(10);
        }
    }

    if($int_only_arg == true){
        if(($parse_only_arg == true) or ($parse_script_arg == true)){
            print("Unallowed combination of arguments!\n");
            exit(10);
        }
    }
}

$Options = new Options();
$test_directories_paths = [];
check_arguments($argc, $argv, $Options);


if($Options->recursive){
    array_push($test_directories_paths, $Options->directory);
    $paths = recursive_get_paths($Options->directory);
    foreach($paths as $path){
        if(is_dir($path)){
            $path = explode($Options->directory, $path)[1];
            $path = $Options->directory.$path;
            array_push($test_directories_paths, $path);
        }else continue;
    }
}else{
    array_push($test_directories_paths, $Options->directory);
}

if($Options->parse_only){
    if(file_exists($Options->parse_script) == FALSE){
        fwrite(STDERR, "Parse script not found at \"".$Options->parse_script."\" !\n");
        exit(11);
    }
    $results = run_test($Options, "parse", $test_directories_paths);
}elseif($Options->int_only){
    if(file_exists($Options->int_script) == FALSE){
        fwrite(STDERR, "Interpret script not found at \"".$Options->int_script."\" !\n");
        exit(11);
    }
    $results = run_test($Options, "int", $test_directories_paths);
}else{
    if(file_exists($Options->parse_script) == FALSE){
        fwrite(STDERR, "Parse script not found at \"".$Options->parse_script."\" !\n");
        exit(11);
    }
    if(file_exists($Options->int_script) == FALSE){
        fwrite(STDERR, "Interpret script not found at \"".$Options->int_script."\" !\n");
        exit(11);
    }
    $results = run_test($Options, "both", $test_directories_paths);
}

$stats = evaluate_results($results);

create_html($stats, $results);
