${DATE} = Get-Date
${LOG_DIR} = "./log/"
${LOG_FILE_SUFFIX} = ("_" + ${DATE}.ToString("yyyy_MM_dd_HH_mm_ss") + ".log")

${CURRENT_TEST_CASE_DIR} =  "./dummyServer/"
${CURRENT_TEST_CASE_SCRIPT_NAME} = "index"
${TEST_CASE_DIR} = "./testcase/"

${TEST_CASE_FILE_SUFFIX} = ".ps1"

${TEST_CASE_LIST} = @{1="sendDc"; 2="sendEpSky"; 3="sendEpPesp"}


${CURRENT_CASE_PATH} = ${CURRENT_CASE_DIR} + ${CURRENT_CASE_SCRIPT}



function ActivateTest(${testName}){
    #ファイル差し替え
    cp ${TEST_CASE_DIR}/${testName}${TEST_CASE_FILE_SUFFIX} ${CURRENT_TEST_CASE_DIR}/${CURRENT_TEST_CASE_SCRIPT_NAME}${TEST_CASE_FILE_SUFFIX}
}

function ExecTest(${testName}){
    #ここでタスク起動
    .\dummyServer\index.ps1
}


#いつものテスト三点セット
function SetUp(${testName}){
    ActivateTest(${testName})
}

function DoTest (${testName}) {
    ExecTest(${testName}) > (${LOG_DIR} + ${testName} + ${LOG_FILE_SUFFIX})
}

function Teardown(${testName}){
    #ここでタスクキル
}


function LoopAllTest(${testCaseList}) {
    foreach ($i in @(1..${testCaseList}.Count)) {
        ${testName} =  ${testCaseList}.Item(${i})
        echo ("testcase${i}:" + ${testName})

        SetUp( ${testName} )
        DoTest( ${testName} )
        Teardown( ${testName} )
    }
}

function Main {
    LoopAllTest ${TEST_CASE_LIST}
}

Main(${args})







