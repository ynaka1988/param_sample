function CreateCom (${port}){
    ${com} = New-Object System.IO.Ports.SerialPort
    ${com}.PortName  = ${port}
    ${com}.Parity = ([System.IO.Ports.Parity]::None)
    ${com}.BaudRate  = 115200
    ${com}.DtrEnable = $true
    ${com}.RtsEnable = $true
    ${com}.Handshake = [System.IO.Ports.Handshake]::None
    ${com}.NewLine   = "`r"
    ${com}.Encoding  = [System.Text.Encoding]::GetEncoding("Shift_JIS")

    return ${com}
}


function RegisterComEvent(${com}, ${log}){
    $global:logfile = ${log}

    ${action} = {
        param([System.IO.Ports.SerialPort]${sender}, [System.EventArgs]$e)
        ${comout} = ${sender}.ReadExisting()
        Write-Host -NoNewline ${comout}

        Write-Output ${comout} | Out-File -Append $global:logfile
    }

    ${event} = Register-ObjectEvent -InputObject ${com} -EventName "DataReceived" -Action ${action} 

    return ${event}
}


function UnRegisterComEvent(${event}){
    Unregister-Event ${event}.Name
    Remove-Job ${event}.id
}


function LoopKey(${com}) {
    for(;;){
        if([Console]::KeyAvailable){
            ${com}.Write(([Console]::ReadKey($true)).KeyChar)
        }
    }   
}


function main {
    ${com} = CreateCom("COM4")
    ${event} = RegisterComEvent ${com} "console.log"
    ${com}.open()

    Write-Host ${com}.PortName "opened"

    try{
        ${com}.WriteLine('')
        ${com}.WriteLine('hostname')
        ${com}.WriteLine('echo hoge')
        LoopKey(${com})
    }Finally{
        ${com}.close()
        UnRegisterComEvent(${event}) 
        Write-Host "Com closed"
    }
}

#Export-ModuleMember -Function CreateCom
#Export-ModuleMember -Function RegisterComEvent
#Export-ModuleMember -Function UnRegisterComEvent
#Export-ModuleMember -Function LoopKey

main
