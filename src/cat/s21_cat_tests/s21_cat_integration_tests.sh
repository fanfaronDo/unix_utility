#!/bin/bash

success_all=0
fail_all=0
cnt_all=0
success=0
fail=0
cnt=0
diff=""
dir_files=./files
s21_cat=../s21_cat


short_flags=(
    "b"
    "e"
    "n"
    "s"
    "t"
    "v"
)

long_flags=(
    "-number"
    "-squeeze-blank"
    "-number-nonblank"
)

testing_files=(
    "flags $dir_files/test_1_cat.txt"
    "flags $dir_files/test_2_cat.txt"
    "flags $dir_files/test_3_cat.txt"
    "flags $dir_files/test_1_cat.txt $dir_files/test_2_cat.txt"
)


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
    for flag in "${short_flags[@]}"
        do
        for test in "${testing_files[@]}"
            do
            f="-$flag"
            test_ex $test
        done
    done
    for flag in "${long_flags[@]}"
        do
        for test in "${testing_files[@]}"
            do
            f="-$flag"
            test_ex $test
        done
    done
    
    print_result_testing
}

function two_parametr_test(){
    print_header "Test with two parametrs"
    for flag1 in "${short_flags[@]}"
        do 
        for flag2 in "${short_flags[@]}"
            do 
            if [ $flag1 != $flag2 ]
            then 
                for test in "${testing_files[@]}"
                    do
                    f="-$flag1 -$flag2"
                    test_ex $test
                done
            fi
        done
    done
    for flag1 in "${long_flags[@]}"
        do 
        for flag2 in "${long_flags[@]}"
            do 
            if [ $flag1 != $flag2 ]
            then 
                for test in "${testing_files[@]}"
                    do
                    f="-$flag1 -$flag2"
                    test_ex $test
                done
            fi
        done
    done
    print_result_testing
}
function three_parametr_test(){
    print_header "Test with three parametrs"
    for flag1 in "${short_flags[@]}"
    do 
        for flag2 in "${short_flags[@]}"
        do
            for flag3 in "${short_flags[@]}"
            do
                if [ $flag1 != $flag2 ] && [ $flag2 != $flag3 ] && [ $flag1 != $flag3 ]
                then 
                    for test in "${testing_files[@]}"
                        do
                        f="-$flag1 -$flag2"
                        test_ex $test
                    done
                fi
            done
        done
    done
    for flag1 in "${long_flags[@]}"
    do 
        for flag2 in "${long_flags[@]}"
        do
            for flag3 in "${long_flags[@]}"
            do
                if [ $flag1 != $flag2 ] && [ $flag2 != $flag3 ] && [ $flag1 != $flag3 ]
                then 
                    for test in "${testing_files[@]}"
                        do
                        f="-$flag1 -$flag2 -$flag3"
                        test_ex $test
                    done
                fi
            done
        done
    done
    print_result_testing 
}

function additional_tests(){
    print_header "Test with combined flags"
    for flag1 in "${short_flags[@]}"
    do
        for flag2 in "${short_flags[@]}"
        do
            for flag3 in "${short_flags[@]}"
            do
                if [ $flag1 != $flag2 ] && [ $flag2 != $flag3 ] && [ $flag1 != $flag3 ]
                then 
                    for test in "${testing_files[@]}"
                    do
                        f="-$flag1$flag2$flag3"
                        test_ex $test
                    done
                fi
            done
        done
    done
    print_result_testing
}

test_ex(){
    
    parametr=$(echo $@ | sed "s/flags/$f/g")

    ../s21_cat $parametr > test_s21_cat.log
    cat $parametr > test_cat.log

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
    three_parametr_test
    additional_tests
    print_result_all_testing
}
main 
