var loading = false;
var version = 2;

function UpdateFilter(searchData)
{
    $(".volume-filter").each(function() {
        if (this.checked) {
            searchData['volumes'].push(parseInt(this.value));
        }
    });
}

function EncodeParam(p)
{
    var str = $.trim(p.text);
    if (p.type == 'substantive') {
        str = 'n=' + str;
        if ('case' in p.substantive) {
            str += "+c" + p.substantive.case;
        }
        if ('participle' in p.substantive) {
            str += "+p" + p.substantive.participle;
        }
        if ('number' in p.substantive) {
            str += "+n" + p.substantive.number;
        }
        if ('gender' in p.substantive) {
            str += "+g" + p.substantive.gender;
        }
    } else if (p.type == 'verb') {
        str = 'v=' + str;
        if ('mood' in p.verb) {
            str += "+m" + p.verb.mood;
        }
        if ('voice' in p.verb) {
            str += "+v" + p.verb.voice;
        }
        if ('number' in p.verb) {
            str += "+n" + p.verb.number;
        }
        if ('tense' in p.verb) {
            str += "+t" + p.verb.tense;
        }
        if ('person' in p.verb) {
            str += "+p" + p.verb.person;
        }
    } else if (str) {
        str = 't=' + str;
    } else {
        return null;
    }
    return str;
}

function DecodeParam(p)
{
    var param = {};
    var r = /t=([a-z]+)/i.exec(p);
    if (r) {
        param.type = 'text';
        param.text = r[1].toLowerCase();
        return param;
    }
    r = /n=([a-z]+.*)/i.exec(p);
    if (r) {
        param.type = 'substantive';
        param.substantive = {};
        param.text = r[1].toLowerCase();
        r = /\+c([0-9]+)/.exec(p);
        if (r) {
            param.substantive.case = r[0];
        }
        r = /\+p([0-9]+)/.exec(p);
        if (r) {
            param.substantive.participle = r[0];
        }
        r = /\+n([0-9]+)/.exec(p);
        if (r) {
            param.substantive.number = r[0];
        }
        r = /\+g([0-9]+)/.exec(p);
        if (r) {
            param.substantive.gender = r[0];
        }
        return param;
    }
    r = /v=([a-z]+).*/i.exec(p);
    if (r) {
        param.type = 'verb';
        param.verb = {};
        param.text = r[1].toLowerCase();
        r = /\+m([0-9]+)/.exec(p);
        if (r) {
            param.verb.mood = r[0];
        }
        r = /\+v([0-9]+)/.exec(p);
        if (r) {
            param.verb.voice = r[0];
        }
        r = /\+n([0-9]+)/.exec(p);
        if (r) {
            param.verb.number = r[0];
        }
        r = /\+t([0-9]+)/.exec(p);
        if (r) {
            param.verb.tense = r[0];
        }
        r = /\+p([0-9]+)/.exec(p);
        if (r) {
            param.verb.person = r[0];
        }
        return param;
    }
    return null;
}

function BuildURI(q)
{
    var params = [];
    $.each(q.params, function(i, param) {
        var p = EncodeParam(param);
        if (p != null) {
            params.push(p);
        }
    });
    if (!q.proximity.enable) {
        params.push('p=a');
    } else if (q.proximity.words != 25) {
        params.push('p=' + q.proximity.words);
    }
    if (q.volumes && q.volumes.length) {
        params.push('f=' + q.volumes.join());
    }
    if (q.count != 10) {
        params.push('c=' + q.count);
    }
    if (q.first != 0) {
        params.push('s=' + q.first);
    }
    return "?" + params.join(';');
}

function ParseURI(uri)
{
    var q = {};
    q.version = version;
    q.proximity = {};
    q.proximity.enable = true;
    q.proximity.words = 25;
    q.count = 10;
    q.first = 0;
    q.volumes = [];
    q.params = [];
    var r = null;
    $.each(uri.split(';'), function (i, param) {
        r = /(v|n|t)=.*/.exec(param);
        if (r) {
            var p = DecodeParam(r[0]);
            if (p) {
                q.params.push(p); 
            }
            return;
        }
        r = /p=a/.exec(param);
        if (r) {
            q.proximity.enable = false;
            return;
        }
        r = /p=([0-9]+)/.exec(param);
        if (r) {
            q.proximity.enable = true;
            q.proximity.words = parseInt(r[1]);
            return;
        }
        r = /f=([0-9,]+)/.exec(param);
        if (r) {
            for (var vol in r[1].split(',')) {
                q.volumes.push(parseInt(vol));
            }
            return;
        }
        r = /c=([0-9]+)/.exec(param);
        if (r) {
            q.count = parseInt(r[1]);
            return;
        }
        r = /s=([0-9]+)/.exec(param);
        if (r) {
            q.first = parseInt(r[1]);
            return;
        }
    });
    return q;
}

function ParseUI() {
    var searchData = {};
    searchData.version = version;
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
    searchData.proximity.enable = $('input#prox-all').is(':checked');
    searchData.proximity.words = parseInt($('input[name="search-proximity"]').val());
    searchData['volumes'] = [];
    UpdateFilter(searchData);

    return searchData;
}

function ApplyUI(q) {
    $('.search-term').remove();
    $(".btn-remove-term").remove();
    if (q.params.length == 0) {
        AddTerm('text');
        return;
    }
    $.each(q.params, function (i, p) {
        if (AddTerm(p.type)) {
            var t = $('.search-term').last();
            t.find('.search-word').first().val(p.text);
            if (p.type == 'substantive') {
            } else if (p.type == 'verb') {
            }
        }
    });
}

function LoadResults(q, d) {
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
        RunQuery(ParseUI());
    });
    $('#resetFilter').on('click', function(event) {
        $(".volume-filter").each(function() {
            this.checked = false;
        });
        RunQuery(ParseUI());
    });
    var results = "";
    $.each(d.passages, function(i, result) {
        results += '<h5><a>' + result.volume + ': ' + result.passage + ' (' + result.count + ' hit' + ((result.count == 1) ? '' : 's') + ')</a></h5>\n'
        results += '<div class="passage">' + result.text + '</div>\n'
        results += '<hr class="spacer"></hr>\n';
    });
    $('#results').html(results);
    var pageCount = Math.floor(d.count / q.count);
    if (pageCount > 0) {
        $('#pageSelection').bootpag({
            total: pageCount,
            page: q.first / q.count + 1,
            maxVisible: 10
        }).off("page").on("page", function(event, page) {
            if (!loading) {
                var newQuery = $.extend({}, q);
                newQuery.first = (page - 1) * newQuery.count;
                RunQuery(newQuery);
            }
        });
    } else {
        $('#pageSelection').html('');
    }
    loading = false;
}

function AddTerm(termType) {
    if ($('.search-term').length >= 16) {
        return false;
    }
    var typeName = 'Text';
    if (termType == 'substantive') {
        typeName = 'Nonverb';
    } else if (termType == 'verb') {
        typeName = 'Verb';
    }

    var newTerm =
      '<div class="input-group search-term">\n' +
        '<input type="text" class="search-word form-control" aria-label="..." size="15">\n' +
        '<div class="input-group-btn">\n' +
          '<button type="button" class="btn btn-default dropdown-toggle" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">' + typeName + ' <span class="caret"></span></button>\n' +
          '<ul class="dropdown-menu dropdown-menu-right dropdown-form">\n' +
            '<li class="dropdown">\n' +
            '<div class="row">\n' +
              '<div class="col-md-12">\n' +
                '<form>\n' +
                  '<label for="grammarEnable">Search mode:</label>\n' +
                  '<div class="form-group" id="grammarEnable">\n' +
                    '<div class="btn-group btn-group-justified">\n' +
                      '<label class="btn btn-default">\n' +
                        '<input type="radio" name="term-type" id="term-type-text" value="Text"' + (termType == 'text' ? ' checked' : '') + '> Text\n' +
                      '</label>\n' +
                      '<label class="btn btn-default">\n' +
                        '<input type="radio" name="term-type" id="term-type-verb" value="Verb"' + (termType == 'verb' ? ' checked' : '') + '> Verb\n' +
                      '</label>\n' +
                      '<label class="btn btn-default">\n' +
                        '<input type="radio" name="term-type" id="term-type-nonverb" value="Nonverb"' + (termType == 'substantive' ? ' checked' : '') + '> Nonverb\n' +
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
            RunQuery(ParseUI());
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
        return false;
    });

    return true;
}

function UpdateHistory(q, d) {
    var terms = [];
    $.each(q.params, function (i, p) {
        var stripped = $.trim(p.text);
        if (stripped) {
            terms.push(stripped); 
        }
    });
    var uri = BuildURI(q);
    var desc = terms.join(' ');
    if (desc) {
        desc += " - "; 
    }
    desc += "NeoSearch";
    $('title').html(desc);
    $('a[name="a"]')[0].scrollIntoView();
    history.pushState({'query':q, 'results':d}, desc, uri);
}

function RunQuery(query_data) {
    if (query_data.params.length == 0) {
        return;
    }
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
                LoadResults(query_data, d);
                UpdateHistory(query_data, d);
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

function LoadURI() {
    var urlParts = window.location.href.split('?');
    if (urlParts.length == 2) {
        var q = ParseURI(urlParts[1]);
        ApplyUI(q);
        RunQuery(q);
    }
}

function UIInit() {
    AddTerm('text');
    $('#pageSelection').bootpag({total: 0, maxVisible: 10});
    $('#searchSubmit').on('click', function(event) {
        RunQuery(ParseUI());
    });
    $('input[type=radio]').on('change', function (event) {
        $(this).parent().addClass("active").siblings().removeClass("active");
    });
    $('#proximityMenu').collapse($('input[name="proximity-mode"]').val() == 'all' ? 'show' : 'hide');
    $('input[name="proximity-mode"]').change( function() {
        $('#proximityMenu').collapse($(this).val() == 'all' ? 'show' : 'hide');
    });

    $('#addTerm').on('click', function(event) {
        AddTerm('text');
        return false;
    });
    $('.dropdown-menu').on('click', 'li', function(event) {
        event.stopPropagation(); 
    });
    window.onpopstate = function(event) {
        if (event.state) {
            LoadResults(event.state.query, event.state.results);
        } else {
            LoadURI(); 
        }
    }
}

$(document).ready(function(){
    UIInit();
    LoadURI(); 
});
