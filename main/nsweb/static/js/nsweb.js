var lastQuery = null;
var loading = false;

function UpdateFilter(searchData)
{
    $(".volume-filter").each(function() {
        if (this.checked) {
            searchData['volumes'].push(parseInt(this.value));
        }
    });
}

function ParseQuery() {
    var searchData = {};
    searchData.version = 2;
    searchData.first = 0;
    searchData.count = 10;
    searchData.params = [];
    $('.search-term').each(function() {
        var term = {};
        var searchType = '';
        $(this).find('input[name="term-type"]').each(function() {
            if ($(this).is(":checked")) {
                searchType = $(this).val();
                return false;
            }
        });
        if (searchType == 'Nonverb') {
            term.type = 'substantive';
            term.substantive = {};
            var nonverbCase = parseInt($(this).find('#nonverbCase').first().val());
            if (!isNaN(nonverbCase)) {
                term.substantive.case = nonverbCase;
            }
            var participle = parseInt($(this).find('#nonverbParticiple').first().val());
            if (!isNaN(participle)) {
                term.substantive.participle = participle;
            }
            var number = parseInt($(this).find('#nonverbNumber').first().val());
            if (!isNaN(number)) {
                term.substantive.number = number;
            }
            var gender = parseInt($(this).find('#nonverbGender').first().val());
            if (!isNaN(gender)) {
                term.substantive.gender = gender;
            }
        } else if (searchType == 'Verb') {
            term.type = 'verb';
            term.verb = {};
            var mood = parseInt($(this).find('#verbMood').first().val());
            if (!isNaN(mood)) {
                term.verb.mood = mood;
            }
            var voice = parseInt($(this).find('#verbVoice').first().val());
            if (!isNaN(voice)) {
                term.verb.voice = voice;
            }
            var number = parseInt($(this).find('#verbNumber').first().val());
            if (!isNaN(number)) {
                term.verb.number = number;
            }
            var tense = parseInt($(this).find('#verbTense').first().val());
            if (!isNaN(tense)) {
                term.verb.tense = tense;
            }
            var person = parseInt($(this).find('#verbPerson').first().val());
            if (!isNaN(person)) {
                term.verb.person = person;
            }
        } else {
            term.type = 'text';
        }

        term.text = $(this).find('.search-word').first().val();
        searchData.params.push(term);
    });
    searchData.proximity = {};
    searchData.proximity.enable = ($('input[name="proximity-mode"]').val() == 'all');
    searchData.proximity.words = parseInt($('input[name="search-proximity"]').val());
    searchData['volumes'] = [];
    UpdateFilter(searchData);

    return searchData;
}

function LoadResults(d) {
    loading = true;
    var bookResults = "";
    $.each(d.volumes, function (i, vol) {
        if (vol.hits > 0) {
            bookResults += '<label class="btn btn-default btn-badged-checkbox">';
            bookResults += '<input class="volume-filter" type="checkbox" value="' + vol.id + '"' + (vol.active ? ' checked' : '') +' ><span>' + vol.name + '</span>';
            bookResults += '<span class="badge">' + vol.hits + '</span></label>\n';
        }
    });
    bookResults += '<label id="resetFilter" class="btn btn-default">Reset Filter</label>';
    $('#resultsByBook').html(bookResults); 
    $('.volume-filter').each( function () {
        $(this).parent().toggleClass("active", $(this).is(':checked'));
    });

    $(".volume-filter").on('click', function(event) {
        RunQuery(ParseQuery());
    });
    $('#resetFilter').on('click', function(event) {
        $(".volume-filter").each(function() {
            this.checked = false;
        });
        RunQuery(ParseQuery());
    });
    var results = "";
    $.each(d.passages, function(i, result) {
        results += '<h5><a>' + result.volume + ': ' + result.passage + ' (' + result.count + ' result' + ((result.count == 1) ? '' : 's') + ')</a></h5>\n'
        results += '<div class="passage">' + result.text + '</div>\n'
        results += '<hr class="spacer"></hr>\n';
    });
    $('#results').html(results);
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

function AddTerm(removable) {
    if ($('.search-term').length >= 16) {
        return;
    }

    var newTerm =
      '<div class="input-group search-term">\n' +
        '<input type="text" class="search-word form-control" aria-label="..." size="15">\n' +
        '<div class="input-group-btn">\n' +
          '<button type="button" class="btn btn-default dropdown-toggle" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">Text <span class="caret"></span></button>\n' +
          '<ul class="dropdown-menu dropdown-menu-right dropdown-form">\n' +
            '<li class="dropdown">\n' +
            '<div class="row">\n' +
              '<div class="col-md-12">\n' +
                '<form>\n' +
                  '<label for="grammarEnable">Search mode:</label>\n' +
                  '<div class="form-group" id="grammarEnable">\n' +
                    '<div class="btn-group btn-group-justified">\n' +
                      '<label class="btn btn-default">\n' +
                        '<input type="radio" name="term-type" id="term-type-text" value="Text" checked> Text\n' +
                      '</label>\n' +
                      '<label class="btn btn-default">\n' +
                        '<input type="radio" name="term-type" id="term-type-verb" value="Verb"> Verb\n' +
                      '</label>\n' +
                      '<label class="btn btn-default">\n' +
                        '<input type="radio" name="term-type" id="term-type-nonverb" value="Nonverb"> Nonverb\n' +
                      '</label>\n' +
                    '</div>\n' +
                  '</div>\n' +
                  '<div class="collapse list-group-submenu" id="verbMenu">\n' +

                    '<div class="form-group grammar-select">\n' +
                      '<label for="verbMood">Mood:</label>\n' +
                      '<select class="form-control grammar-select" id="verbMood">\n"' +
                        '<option value="">any</option>\n' +
                        '<option value="1">indicative</option>\n' +
                        '<option value="2">subjunctive</option>\n' +
                        '<option value="3">infinitive</option>\n' +
                        '<option value="4">imperative</option>\n' +
                        '<option value="0">none</option>\n' +
                      '</select>\n' +
                    '</div>\n' +

                    '<div class="form-group grammar-select">\n' +
                      '<label for="verbVoice">Voice:</label>\n' +
                      '<select class="form-control grammar-select" id="verbVoice">\n"' +
                        '<option value="">any</option>\n' +
                        '<option value="1">active</option>\n' +
                        '<option value="2">passive</option>\n' +
                        '<option value="0">none</option>\n' +
                      '</select>\n' +
                    '</div>\n' +

                    '<div class="form-group grammar-select">\n' +
                      '<label class="control-label" for="verbNumber">Number:</label>\n' +
                      '<select class="form-control grammar-select" id="verbNumber">\n' +
                        '<option value="">any</option>\n' +
                        '<option value="1">singular</option>\n' +
                        '<option value="2">plural</option>\n' +
                        '<option value="0">none</option>\n' +
                      '</select>\n' +
                    '</div>\n' +

                    '<div class="form-group grammar-select">\n' +
                      '<label class="control-label" for="verbTense">Tense:</label>\n' +
                      '<select class="form-control grammar-select" id="verbTense">\n' +
                        '<option value="">any</option>\n' +
                        '<option value="1">present</option>\n' +
                        '<option value="2">imperfect</option>\n' +
                        '<option value="3">future</option>\n' +
                        '<option value="4">perfect</option>\n' +
                        '<option value="5">pluperfect</option>\n' +
                        '<option value="5">future perfect</option>\n' +
                        '<option value="0">none</option>\n' +
                      '</select>\n' +
                    '</div>\n' +

                    '<div class="form-group grammar-select">\n' +
                      '<label class="control-label" for="verbPerson">Person:</label>\n' +
                      '<select class="form-control grammar-select" id="verbPerson">\n' +
                        '<option value="">any</option>\n' +
                        '<option value="1">1st</option>\n' +
                        '<option value="2">2nd</option>\n' +
                        '<option value="3">3rd</option>\n' +
                        '<option value="0">none</option>\n' +
                      '</select>\n' +
                    '</div>\n' +

                  '</div>\n' +
                  '<div class="collapse list-group-submenu" id="nonverbMenu">\n' +

                    '<div class="form-group grammar-select">\n' +
                      '<label class="control-label" for="nonverbCase">Case:</label>\n' +
                      '<select class="form-control grammar-select" id="nonverbCase">\n' +
                        '<option value="">any</option>\n' +
                        '<option value="1">nominative</option>\n' +
                        '<option value="2">genitive</option>\n' +
                        '<option value="3">dative</option>\n' +
                        '<option value="4">accusative</option>\n' +
                        '<option value="5">ablative</option>\n' +
                        '<option value="6">vocative</option>\n' +
                        '<option value="7">locative</option>\n' +
                        '<option value="0">none</option>\n' +
                      '</select>\n' +
                    '</div>\n' +

                    '<div class="form-group grammar-select">\n' +
                      '<label class="control-label" for="nonverbParticiple">Participle:</label>\n' +
                      '<select class="form-control grammar-select" id="nonverbParticiple">\n' +
                        '<option value="">any</option>\n' +
                        '<option value="1">substantive</option>\n' +
                        '<option value="2">present</option>\n' +
                        '<option value="3">perfect active</option>\n' +
                        '<option value="4">perfect passive</option>\n' +
                        '<option value="5">gerundive</option>\n' +
                        '<option value="6">present active</option>\n' +
                        '<option value="7">future active</option>\n' +
                        '<option value="8">comparative of adverb or adjective</option>\n' +
                        '<option value="9">superlative of adverb or adjective</option>\n' +
                        '<option value="10">comparative of perfect passive</option>\n' +
                        '<option value="11">superlative of perfect passive</option>\n' +
                        '<option value="12">comparative of present active</option>\n' +
                        '<option value="13">superlative of present active</option>\n' +
                        '<option value="0">none</option>\n' +
                      '</select>\n' +
                    '</div>\n' +

                    '<div class="form-group grammar-select">\n' +
                      '<label class="control-label" for="nonverbGender">Gender:</label>\n' +
                      '<select class="form-control grammar-select" id="nonverbGender">\n' +
                        '<option value="">any</option>\n' +
                        '<option value="2">feminine</option>\n' +
                        '<option value="1">masculine</option>\n' +
                        '<option value="3">neuter</option>\n' +
                        '<option value="0">none</option>\n' +
                      '</select>\n' +
                    '</div>\n' +

                    '<div class="form-group grammar-select">\n' +
                      '<label class="control-label" for="nonverbNumber">Number:</label>\n' +
                      '<select class="form-control grammar-select" id="nonverbNumber">\n' +
                        '<option value="">any</option>\n' +
                        '<option value="1">singular</option>\n' +
                        '<option value="2">plural</option>\n' +
                        '<option value="0">none</option>\n' +
                      '</select>\n' +
                    '</div>\n' +

                  '</div>\n' +
                '</form>\n' +
              '</div>\n' +
            '</div>\n' +
            '</li>\n' +
          '</ul>\n' +
        '</div><!-- /btn-group -->\n' +
      '</div><!-- /input-group -->';

    if ($('.search-term').length == 1) {
        newTerm =
          '<a href="#" class="btn btn-remove-term" id="remove_term">\n' +
            '<span class="glyphicon glyphicon-remove"></span>\n' +
          '</a>\n' + newTerm;
    }

    if ($('.search-term').length > 0) {
        newTerm += '\n' +
          '<a href="#" class="btn btn-remove-term" id="remove_term">\n' +
            '<span class="glyphicon glyphicon-remove"></span>\n' +
          '</a>';
    }

    $("#searchTerms").append(newTerm);

    $('.volume-filter, input[type=radio]').each( function () {
        $(this).parent().toggleClass("active", $(this).is(':checked'));
    });

    $('input.search-word').off('keypress').on('keypress', function(event) {
        if (event.which == 13) {
            RunQuery(ParseQuery());
            return false;
        }
    });

    $('input[name="term-type"]').off('change').on('change', function(event) {
        $(this).parent().addClass("active").siblings().removeClass("active");
        var searchTerm = $(this).closest('.search-term');
        searchTerm.find('button.dropdown-toggle').first().html($(this).val() + ' <span class="caret">');
        searchTerm.find('#verbMenu').collapse($(this).val() == 'Verb' ? 'show' : 'hide');
        searchTerm.find('#nonverbMenu').collapse($(this).val() == 'Nonverb' ? 'show' : 'hide');
    });

    $(".btn-remove-term").off("click").on("click", function (event) {
        $(this).prev().remove();

        if ($('.search-term').length < 2) {
            $(".btn-remove-term").remove();
        } else {
            $(this).remove();
        }
    });
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
                $('#resultHeader').text("" + d.total_hits + " hit" + (d.total_hits == 1 ? '' : 's') + " (" + ((end - start) / 1000).toFixed(1) + " seconds)");
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
    AddTerm();
    $('#pageSelection').bootpag({total: 0, maxVisible: 10});
    $('#searchSubmit').on('click', function(event) {
        RunQuery(ParseQuery());
    });
    $('input[type=radio]').on('change', function (event) {
        $(this).parent().addClass("active").siblings().removeClass("active");
    });
    $('#proximityMenu').collapse($('input[name="proximity-mode"]').val() == 'all' ? 'show' : 'hide');
    $('input[name="proximity-mode"]').change( function() {
        $('#proximityMenu').collapse($(this).val() == 'all' ? 'show' : 'hide');
    });

    $('#addTerm').on('click', function(event) {
        AddTerm();
    });
    $('.dropdown-menu').on('click', 'li', function(event) {
        event.stopPropagation(); 
    });
}

$(document).ready(function(){
    UIInit();
});
