var lastQuery = null;
var loading = false;

function ParseQuery() {
    var searchData = {};
    searchData['version'] = 2;
    searchData['first'] = 0;
    searchData['count'] = 10;
    searchData['params'] = [];
    searchData['params'].push($('input#textInput0').val());
    return searchData;
}

function LoadResults(d) {
    loading = true;
    var bookResults = "";
    $.each(d.volumes, function (i, vol) {
        if (vol.count > 0) {
            bookResults += '<label class="btn btn-default btn-badged-checkbox">';
            //bookResults += '<input type="checkbox"><span>' + vol.name + '</span>';
            bookResults += '<span>' + vol.name + '</span>';
            bookResults += '<span class="badge">' + vol.count + '</span></label>\n';
        }
    });
    $('div#resultsByBook').html(bookResults); 
    $('#pageSelection').bootpag({
        total: (d.count + lastQuery.count - 1) / lastQuery.count,
        page: lastQuery.first / lastQuery.count + 1,
        maxVisible: 10
    }).off("page").on("page", function(event, page) {
        if (!loading) {
            var newQuery = $.extend({}, lastQuery);
            newQuery.first = (page - 1) * newQuery.count;
            RunQuery(newQuery);
        }
    });
    loading = false;
}

function RunQuery(query_data) {
    lastQuery = query_data;
    var str_query = JSON.stringify(query_data);
    if (Modernizr.localstorage) {
        localStorage.setItem("last_query", str_query);
    }
    var start = new Date().getTime();  
    $.ajax({ url: "/Search/", method: "POST", contentType: "application/json", data: str_query}).done(function(d) {
            /*
            var mismatch = (d.version != clientVersion);
            if (mismatch) {
            }
            */
            if (d.errors.length > 0) {
                $('#resultHeader').text(d.errors[0]);
            } else {
                /*
                $('#queryModal .progress').removeClass('active');
                $('#queryModal .progress-bar').addClass('progress-bar-warning');
                $('#queryModal #queryResult').html("Search found 0 results.");
                $('#queryModal #cancel-query-btn').html("Close");
                */

                var end = new Date().getTime();
                $('#resultHeader').text("" + d.count + " results (" + ((end - start) / 1000).toFixed(1) + " seconds)");
                LoadResults(d);
            }
    }).fail(function() {
        /*
        $('#queryModal .progress-bar').addClass('progress-bar-danger');
        $('#queryModal .progress').removeClass('active');
        $('#queryModal #queryResult').html("Search failed");
        $('#queryModal #cancel-query-btn').html("Close");
        */ 
        $('#resultHeader').text("Search failed.");
    });
}

function UIInit() {
    $('#pageSelection').bootpag({total: 0});
    $('#searchSubmit').on('click', function(event) {
        RunQuery(ParseQuery());
    });
}

$(document).ready(function(){
    UIInit();
});
