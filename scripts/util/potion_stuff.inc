/*use uo;*/

////////////////////////////////////////
//
//  We're going to build a 'type' here.
//
var flaskcontentstype array;
flaskcontentstype .+ reg;
flaskcontentstype .+ amount;
//
//  Now, when you want one of these in an array, or a variable, you just
//  write,
//  var variable := FlaskContentsType;
//
////////////////////////////////////////

function write_contents(thecontents, thecontainer)

    // the contents is an array of up to 3 structures, containing .reg and .amount members

    var content_string := "";
    // var counter := 0;
    // var tlen := len(thecontents);

    foreach item in thecontents
        if (item.amount > 0)
            if (content_string)
                content_string := content_string + ",";
            endif

            content_string := item.reg + ConvertNumber(item.amount);
        endif
    endforeach

    if (content_string > "")
        EraseObjProperty(thecontainer, "contents");
        EraseObjProperty(thecontainer, "heated");
    else
        SetObjProperty(thecontainer, "contents", content_string);
    endif

endfunction


function ConvertNumber(thenumber)
    if (thenumber < 10)
        return "0" + CStr(thenumber);
    else
        return CStr(thenumber);
    endif
endfunction

function GetContents(container)

    // reads the properties of a container, and returns an array of structures. Each structure has .reg and .amount members 

    var contents := GetObjProperty(container,"contents");

    if (!contents)
            return 0;
    endif

    var thecontents array;

    var counter;
    var tlen := len(contents);

    if (tlen == 0)
            return -1;
    endif

    var num_regs := 0;
    var tempstr := "";

    for (counter := 1; counter <= tlen; counter := counter + 1)
        if (counter == 1)
                num_regs := num_regs + 1;
                thecontents[num_regs] := flaskcontentstype;
                thecontents[num_regs].reg := contents[counter];
        elseif (thecontents[counter - 1] == ",")
                num_regs := num_regs + 1;
                thecontents[num_regs] := flaskcontentstype;
                thecontents[num_regs].reg := contents[counter];
                thecontents[num_regs - 1].amount := CInt(tempstr);
                tempstr := "";
        else
                tempstr := tempstr + thecontents[counter];
                if (counter == tlen)
                        thecontents[num_regs] := flaskcontentstype;
                        thecontents[num_regs].amount := CInt(tempstr);
                endif
        endif
    endfor

return thecontents;

endfunction


function AddRegToFlask(flask, reg, amount)

    var contents := GetContents(flask);

    if (amount > 99)
            amount := 99;
    endif

    if (contents == -1)
            var stuff := flaskcontentstype;
            stuff[1].reg := reg;
            stuff[1].amount := amount;
            write_contents(stuff, flask);
    else

        var i;
        var tlen := len(contents);
        var inthere := 0;


        while (i <= tlen)
                i := i + 1;
                if (contents[i] == reg)
                        inthere := 1;
                        break;
                endif
        endwhile

        if (inthere)
                contents[i].amount := contents[i].amount + amount;
        else
                contents[tlen + 1].reg := reg;
                contents[tlen + 1].amount := amount;
        endif

        write_contents(contents, flask);

    endif
endfunction

function TakeRegsFromFlask(flask, reg, amount)

    var contents := GetContents(flask);

    if (contents == -1) //the flask is empty
            return -1;
    else

        var i;
        var tlen := len(contents);
        var inthere := 0;


        while (i <= tlen)
                i := i + 1;
                if (contents[i] == reg)
                        inthere := 1;
                        break;
                endif
        endwhile

        if (inthere)
                contents[i].amount := contents[i].amount - amount;
                if (contents[i].amount < 0)
                        return contents[i];
                endif
        else
                return -1;
        endif

        write_contents(contents, flask);

    endif
endfunction

/* added for testing
program potion_stuff()
var flask := CreateItemAtLocation(1,1,1,0xeed,1);

 AddRegToFlask(flask, "n", 1);
 takeregsfromflask(flask, "n",5);
 
endprogram
*/