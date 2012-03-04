
Partial Class _Default
    Inherits System.Web.UI.Page

    Protected Sub ForwardButton_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles ForwardButton.Click
        Dim FSO, myFile
        FSO = Server.CreateObject("Scripting.FileSystemObject")
        myFile = FSO.CreateTextFile("robotcontrol.json", True)
        myFile.WriteLine("{""cmd"":""forward""}")
        myFile.Close()
    End Sub

    Protected Sub LeftButton_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles LeftButton.Click
        Dim FSO, myFile
        FSO = Server.CreateObject("Scripting.FileSystemObject")
        myFile = FSO.CreateTextFile("robotcontrol.json", True)
        myFile.WriteLine("{""cmd"":""left""}")
        myFile.Close()
    End Sub

    Protected Sub RightButton_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles RightButton.Click
        Dim FSO, myFile
        FSO = Server.CreateObject("Scripting.FileSystemObject")
        myFile = FSO.CreateTextFile("robotcontrol.json", True)
        myFile.WriteLine("{""cmd"":""right""}")
        myFile.Close()
    End Sub

    Protected Sub ReverseButton_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles ReverseButton.Click
        Dim FSO, myFile
        FSO = Server.CreateObject("Scripting.FileSystemObject")
        myFile = FSO.CreateTextFile("robotcontrol.json", True)
        myFile.WriteLine("{""cmd"":""backward""}")
        myFile.Close()
    End Sub

    Protected Sub StopButton_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles StopButton.Click
        Dim FSO, myFile
        FSO = Server.CreateObject("Scripting.FileSystemObject")
        myFile = FSO.CreateTextFile("robotcontrol.json", True)
        myFile.WriteLine("{""cmd"":""stop""}")
        myFile.Close()
    End Sub
End Class
