<?php
/**
 * IPP 1. projekt - parse.php
 * datum: únor 2020
 * autor: Jan Šulavík (xsulav01)
 */

ini_set('display_errors', 'stderr');

$helpmsg = "IPP project 2020 - parse.php
script takes input written in IPPcode20 from STDIN
and produces XML representation to STDOUT\n";

//handle arguments, only one argument '--help' supported
if($argc == 2){
    if($argv[1] == "--help"){
        echo $helpmsg;
        exit(0);
    }else{
        fprintf(STDERR, "unknown argument!\n");
        fprintf(STDERR, "supported argument is --help\n");
        exit(10);
    }
}else if($argc > 2){
    fprintf(STDERR, "too many arguments!\n");
    exit(10);
}

//line counter and first line detector
$lineCount = 0;
$firstLine = TRUE;

while(FALSE !== ($line = fgets(STDIN))) {
    //first remove comments, if line is empty after removing, go to the next one
    $line = removeComments($line);
    if($line == 1){
        continue;
    }
    //empty lines are allowed before header, skip them
    if (ctype_space($line) == TRUE){
        continue;
    }
    $lineCount++;

    //check if the header is present
    if($firstLine == TRUE){
        if(parseHeader($line) == 1){
            fprintf(STDERR, "incorrect or missing header '.IPPcode20'!\n");
            exit(21);
        }else{
            $firstLine = FALSE;
            $lineCount--;
            $xw = makeXMLHeader();
            continue;
        }
    }

    //remove whitespace from the end of the line after removing comments
    $line = ltrim($line);
    $line = rtrim($line);

    $tokens = array_filter(explode (" ", $line)); //make tokens and filter out empty elements
    if($tokens[0] == ""){ //checking if the line is empty
        $lineCount--;
        continue;
    }

    //parse one line of program
    $result = getInstruction($tokens, $xw, $lineCount);
    if($result == 22){
        fprintf(STDERR, "unknown instruction ".$tokens[0]);
        exit(22);
    }elseif($result == 23){
        fprintf(STDERR, "lexical or syntactical error in ".$tokens[0]);
        exit(23);
    }

}

// end element program, end document and print XML to stdout
endXML($xw);

//function loads one instruction and checks its syntax
function getInstruction($tokens, $xw, $lineCount){
    $tokens[0] = strtoupper($tokens[0]);
    $instruction = $tokens[0];
    $instruction = removeWhitespace($instruction);

    //predeclaring array for argument types and variable for holding the argument's value
    $argTypes = [];
    $text = NULL;

    //switch for checking syntax of instructions
    //instructions with the same syntax are grouped together
    switch ($instruction){
            //<var> <symb>
        case "MOVE":
        case "NOT":
        case "STRLEN":
        case "TYPE":
        case "INT2CHAR":
            if(sizeof($tokens) != 3){
                return 23;
            }
            if(varSyntax($tokens[1]) == FALSE){
                return 23;
            }else{
                array_push($argTypes, "var");
            }

            $symArr = symbolSyntax($tokens[2]);
            if($symArr == FALSE) {
                return 23;
            }else if($symArr == "var"){
                array_push($argTypes, "var");
            }else{
                $argTypes = symbTypeAndValue($symArr, $argTypes);
                $text = $symArr[1];
            }

            writeInstruction($xw, $lineCount, $tokens, sizeof($tokens) - 1, $argTypes, $text);
            break;

            //no arguments
        case "CREATEFRAME":
        case "PUSHFRAME":
        case "POPFRAME":
        case "BREAK":
        case "RETURN":
            if(sizeof($tokens) != 1){
                return 23;
            }

            writeInstruction($xw, $lineCount, $tokens, sizeof($tokens) - 1, $argTypes, $text);
            break;

            //<var>
        case "DEFVAR":
        case "POPS":
            if(sizeof($tokens) != 2){
                return 23;
            }
            if(varSyntax($tokens[1]) == FALSE){
                return 23;
            }else{
                array_push($argTypes, "var");
            }

            writeInstruction($xw, $lineCount, $tokens, sizeof($tokens) - 1, $argTypes, $text);
            break;

            //<label>
        case "LABEL":
        case "JUMP":
        case  "CALL":
            if(sizeof($tokens) != 2){
                return 23;
            }
            if(labelSyntax($tokens[1]) == FALSE){
                return 23;
            }else{
                array_push($argTypes, "label");
            }

            writeInstruction($xw, $lineCount, $tokens, sizeof($tokens) - 1, $argTypes, $text);
            break;

            //<symb>
        case "WRITE":
        case "EXIT":
        case "DPRINT":
        case "PUSHS":
            if(sizeof($tokens) != 2){
                return 23;
            }

            $symArr = symbolSyntax($tokens[1]);
            if($symArr == FALSE){
                return 23;
            }else if($symArr == "var"){
                array_push($argTypes, "var");
            }else{
                $argTypes = symbTypeAndValue($symArr, $argTypes);
                $text = $symArr[1];
            }

            writeInstruction($xw, $lineCount, $tokens, sizeof($tokens) - 1, $argTypes, $text);
            break;

            //<var> <symb1> <symb2>
        case "ADD":
        case "SUB":
        case "MUL":
        case "IDIV":
        case "LT":
        case "GT":
        case "EQ":
        case "AND":
        case "OR":
        case "CONCAT":
        case "GETCHAR":
        case "SETCHAR":
        case "STRI2INT":
        if(sizeof($tokens) != 4){
            return 23;
        }

        $text = [];
        array_push($text, null);

        if(varSyntax($tokens[1]) == FALSE){
            return 23;
        }else{
            array_push($argTypes, "var");
        }

        $symArr = symbolSyntax($tokens[2]);
        if(symbolSyntax($tokens[2]) == FALSE){
            return 23;
        }else{
            $argTypes = symbTypeAndValue($symArr, $argTypes);
            array_push($text, $symArr[1]);
        }

        $symArr = symbolSyntax($tokens[3]);
        if(symbolSyntax($tokens[3]) == FALSE){
            return 23;
        }else{
            $argTypes = symbTypeAndValue($symArr, $argTypes);
            array_push($text, $symArr[1]);
        }

        writeInstruction($xw, $lineCount, $tokens, sizeof($tokens) - 1, $argTypes, $text);
        break;

            //<var> <type>
        case  "READ":
            if(sizeof($tokens) != 3){
                return 23;
            }

            if(varSyntax($tokens[1]) == FALSE){
                return 23;
            }else{
                array_push($argTypes, "var");
            }

            $type = typeSyntax($tokens[2]);
            if($type == FALSE){
                return 23;
            }else{
                array_push($argTypes, "type");
                $text = $type;
            }

            writeInstruction($xw, $lineCount, $tokens, sizeof($tokens) - 1, $argTypes, $text);
            break;

            //<label> <symb1> <symb2>
        case  "JUMPIFEQ":
        case  "JUMPIFNEQ":
            if(sizeof($tokens) != 4){
                return 23;
            }

            $text = [];
            array_push($text, null);

            if(labelSyntax($tokens[1]) == FALSE){
                return 23;
            }else{
                array_push($argTypes, "label");
            }

            $symArr = symbolSyntax($tokens[2]);
            if(symbolSyntax($tokens[2]) == FALSE){
                return 23;
            }else{
                $argTypes = symbTypeAndValue($symArr, $argTypes);
                array_push($text, $symArr[1]);
            }

            $symArr = symbolSyntax($tokens[3]);
            if(symbolSyntax($tokens[3]) == FALSE){
                return 23;
            }else{
                $argTypes = symbTypeAndValue($symArr, $argTypes);
                array_push($text, $symArr[1]);
            }

            writeInstruction($xw, $lineCount, $tokens, sizeof($tokens) - 1, $argTypes, $text);
            break;

        default:
            return 22;
    }

}

//function removes whitespace characters in $var
function removeWhitespace($var){
    //removes all space chars
    $var = str_replace(" ", '', $var);

    //removes all newline chars
    $var = str_replace("\n", '', $var);

    //removes all tab characters
    $var = str_replace("\t", '', $var);
    return $var;
}

//function removes all comments from the line
function removeComments($line){
    if(strpos($line, '#') === FALSE){
        return $line;
    }else{
        $line = explode('#', $line);
        $line = $line[0];
        if($line == ""){ //if the line is just comment, return 1
            $line = 1;
            return $line;
        }

        return $line;
    }
}

//function processes of the first line of the program
function parseHeader($line){
    $line = removeComments($line);
    $line = removeWhitespace($line);

    $line = strtoupper($line);
    if(strcmp($line, ".IPPCODE20") != 0){
        return 1;
    }else return 0;
}

//function checks the syntax of <variable>
function varSyntax($var){
    if((preg_match("/^(GF|LF|TF)@[A-Za-z|0-9|_|\-|$|&|%|*|!|?]+$/", $var) === 1)){
        return TRUE;
    }else return FALSE;
}

//function checks the syntax of <symbol>
function symbolSyntax($symbol){
    //check, if the symbol is a variable
    if(preg_match("/^(GF|LF|TF)@[A-Za-z|0-9|_|\-|$|&|%|*|!|?]+$/", $symbol) === 1){
        return "var";
    }elseif(preg_match("/^(bool|string|int|nil)@.*$/", $symbol) === 1){ //else check if it is a constant
        $symbol = explode('@', $symbol);

        //dividing the constant into type and value
        $symType = $symbol[0];
        $symbol = $symbol[1];

        $returnArr = [];

        if($symType == "bool"){             //for type BOOL
            $symbol = strtolower($symbol);
            if(preg_match("/^(false|true)$/", $symbol) === 1){
                array_push($returnArr, $symType, $symbol);
                return $returnArr;
            }else return FALSE;
        }
        elseif($symType == "string"){      //for type STRING
            if(preg_match("/^(\\\\[0-9]{3}|[^\\\\])*$/", $symbol) === 1){
                array_push($returnArr, $symType, $symbol);
                return $returnArr;
            }else return FALSE;
        }elseif($symType == "int"){        //for type INT
            if(preg_match("/^-?[0-9]+$/", $symbol) === 1){
                array_push($returnArr, $symType, $symbol);
                return $returnArr;
            }else return FALSE;
        }elseif($symType == "nil"){     //for type NIL
            if(strcmp($symbol, "nil") == 0){
                array_push($returnArr, $symType, $symbol);
                return $returnArr;
            }else return FALSE;
        }
        return TRUE;
    }else return FALSE;
}

//function checks the syntax of <label>
function labelSyntax($label){
    if(preg_match("/^[A-Za-z]|[0-9]|_|-|\$|&|%|\*|!|\?]+$/", $label) === 1){
        return TRUE;
    }else return FALSE;
}

//function checks the syntax of <type>
function typeSyntax($type){
    if(preg_match("/^(int|string|bool|nil)$/", $type) === 1){
        return $type;
    }else return FALSE;
}

//function recognizes type of <symbol>
function symbTypeAndValue($symArr, $argTypes){
    if($symArr[0] == "bool"){
        array_push($argTypes, "bool");
    }elseif ($symArr[0] == "string"){
        array_push($argTypes, "string");
    }elseif ($symArr[0] == "int"){
        array_push($argTypes, "int");
    }elseif($symArr[0] == "nil"){
        array_push($argTypes, "nil");
    }

    return $argTypes;
}

//function creates XML file header and returns the XML resource
function makeXMLHeader(){
    $xw = xmlwriter_open_memory();
    if($xw == FALSE){
        fprintf(STDERR, "internal error!\n");
        exit(99);
    }

    xmlwriter_set_indent($xw, 1);
    xmlwriter_set_indent_string($xw, '    ');

    xmlwriter_start_document($xw, '1.0', 'UTF-8');
    xmlwriter_start_element($xw, 'program');
    xmlwriter_write_attribute($xw, "language", "IPPcode20");

    return $xw;
}

//function writes instruction into the XML
//$order - number of the instruction
//$tokens - token array
//$argsNum - number of arguments to be printed
//$argTypes - array holding types of arguments
//$text - holds value of argument
function writeInstruction($xw, $order, $tokens, $argsNum, $argTypes, $text){
    //removing whitespaces from all tokens
    for($i = 0; $i < sizeof($tokens); $i++){
        $tokens[$i] = removeWhitespace($tokens[$i]);
    }

    xmlwriter_start_element($xw, 'instruction');
    xmlwriter_write_attribute($xw, "order", $order);
    xmlwriter_write_attribute($xw, "opcode", $tokens[0]);

    $y = 0;
    if(is_array($text)){
        for($x = 1; $x <= $argsNum; $x++){
            writeArgs($xw, $tokens[$x], $argTypes[$y], $x, $text[$y]);
            $y++;
        }
    }else{
        for($x = 1; $x <= $argsNum; $x++){
            writeArgs($xw, $tokens[$x], $argTypes[$y], $x, $text);
            $y++;
        }
    }

    xmlwriter_end_element($xw);
}

//function writes arguments of instruction into the XML
function writeArgs($xw, $token, $argType, $argNum, $text){
    xmlwriter_start_element($xw, 'arg'.$argNum);
    xmlwriter_write_attribute($xw, "type", $argType);

    $text = removeWhitespace($text);

    if($argType == "var") {
        xmlwriter_text($xw, $token);
    }elseif($argType == "label"){
        xmlwriter_text($xw, $token);
    }else{
        xmlwriter_text($xw, $text);
    }

    xmlwriter_end_element($xw);
}

//function ends element 'program' and prints the output to STDOUT
function endXML($xw){
    xmlwriter_end_element($xw);
    xmlwriter_end_document($xw);
    echo xmlwriter_output_memory($xw);
}