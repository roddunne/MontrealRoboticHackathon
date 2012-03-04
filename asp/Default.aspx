<%@ Page Language="VB" AutoEventWireup="false" CodeFile="Default.aspx.vb" Inherits="_Default" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>Untitled Page</title>
</head>
<body>
    <form id="form1" runat="server">
    <div>
        <asp:PlaceHolder ID="PlaceHolder1" runat="server"></asp:PlaceHolder>
        <asp:Button ID="ForwardButton" runat="server" Text="Forward" />
        &nbsp;<br />
        <asp:Button ID="LeftButton" runat="server" Text="Left" />
        <asp:Button ID="RightButton" runat="server" Text="Right" />
        <br />
        <asp:PlaceHolder ID="PlaceHolder2" runat="server"></asp:PlaceHolder>
        <asp:Button ID="ReverseButton" runat="server" Text="Reverse" />
        <br />
        <asp:Button ID="StopButton" runat="server" Text="Stop" />
        </div>
    </form>
</body>
</html>
