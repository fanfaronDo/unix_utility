#!/bin/bash

success_all=0
fail_all=0
cnt_all=0
success=0
fail=0
cnt=0
diff=""
dir_files=./files


flags=(
    "i"
    "v"
    "c"
    "l"
    "n"
    "h"
    "s"
    "o" 
    "f"
    "e"
)

testing_files=(
    "flags $dir_files/test_1_grep.txt"
    "flags $dir_files/test_2_grep.txt"
    "flags $dir_files/test_3_grep.txt"
    "flags $dir_files/no_files.txt"
    "flags $dir_files/test_1_grep.txt $dir_files/test_2_grep.txt"
)



testing_pattern="char"


file_pattern=".\/files\/test_ptrn_grep.txt"


function print_header(){
    echo "########################################################################"
    echo "                          $1"
    echo "########################################################################"
}

function print_result_all_testing(){
    echo "________________________________________________________________________"
    echo ""
    echo FAIL: $fail_all
    echo SUCCESS: $success_all
    echo ALL TEST: $cnt_all
    echo "________________________________________________________________________"
}

function print_result_testing(){
    echo "________________________________________________________________________"
    echo ""
    echo FAIL: $fail
    echo SUCCESS: $success
    echo TESTS: $cnt 
    echo "________________________________________________________________________"
    (( cnt = 0 ))
    (( success = 0 ))
    (( fail = 0 )) 
}

function print_result_test(){
    echo "$cnt: $1: $2"
}


function one_parametr_test(){
    print_header "Test with one parametr"
    for flag in "${flags[@]}"
        do
        for test in "${testing_files[@]}"
            do
            f="-$flag "
            if [ $flag == "f" ]
            then
                f+="$file_pattern"        
            else
                f+="$testing_pattern"
            fi
            test_ex $test
        done
    done
    print_result_testing
}

function build_parametr(){
    f+="-"
    if [ $1 == "f" ]
    then
        f+="$1 $file_pattern "
    elif [ $1 == "e" ]
    then
        f+="$1 $testing_pattern "
    else
        f+="$1 "
    fi
}

function two_parametr_test(){
    print_header "Test with two parametrs"
    
    for flag1 in "${flags[@]}"
        do 
        for flag2 in "${flags[@]}"
            do 
            if [ $flag1 != $flag2 ]
            then 
                for test in "${testing_files[@]}"
                do
                    f=""
                    build_parametr "$flag1"
                    build_parametr "$flag2"
                    if [ $flag1 != "e" ] && [ $flag2 != "e" ] && [ $flag1 != "f" ] && [ $flag2 != "f" ] 
                    then 
                        f+="$testing_pattern "
                    fi

                    test_ex $test
                done
            fi
        done
    done
    print_result_testing
}

function three_related_parametr_test(){
    print_header "Test with three parametrs"
    for flag1 in "${flags[@]}"
    do 
        for flag2 in "${flags[@]}"
        do
            for flag3 in "${flags[@]}" 
            do
                if [ $flag1 != $flag2 ] && [ $flag2 != $flag3 ] && [ $flag1 != $flag3 ]
                then 
                    for test in "${testing_files[@]}"
                    do
                        if [ $flag1 != "f" ] && [ $flag1 != "e" ] &&
                        [ $flag2 != "f" ] && [ $flag2 != "e" ]
                        then
                            if [ $flag3 == "f" ]
                            then
                                f="-$flag1$flag2$flag3 $file_pattern"
                            else
                                f="-$flag1$flag2$flag3 $testing_pattern"
                            fi
                            test_ex $test
                        fi
                    done
                fi
            done
        done
    done
    print_result_testing
}

test_ex(){
    
    parametr=$(echo $@ | sed -e "s/flags/$f/g")
    ../s21_grep $parametr > test_s21_cat.log
    grep $parametr > test_cat.log

    diffrent_files="$(diff -s test_s21_cat.log test_cat.log)"
    ((cnt++))
    ((cnt_all++))
    
    if [ "$diffrent_files" == "Files test_s21_cat.log and test_cat.log are identical" ]
    then 
        ((success_all++))
        ((success++))
        print_result_test "SUCCESS" "$parametr"
    else
        ((fail_all++))
        ((fail++))
        print_result_test "FAIL" "$parametr"
    fi
    rm test_s21_cat.log test_cat.log   
}

function main() {
    one_parametr_test
    two_parametr_test
    three_related_parametr_test
    print_result_all_testing
}
main 
