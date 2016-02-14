#!python2

import os
import time

from flask import Flask, render_template, jsonify, request, make_response
from flask.ext.assets import Environment, Bundle

import nsweb

app = Flask(__name__)

assets = Environment(app)
version = 1
app.debug = True
app.config['ASSETS_DEBUG'] = app.debug
app.config['PROPAGATE_EXCEPTIONS'] = True
app.config['UGLIFYJS_BIN'] = '/usr/local/bin/uglifyjs'
app.config['UGLIFYJS_EXTRA_ARGS'] = ['--comments', '/(MIT|license)/']

import logging

logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
ch.setFormatter(formatter)
logger.addHandler(ch)

from jsonschema import validate, ValidationError
import json
schema_filename = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'static', 'schema', 'ns.json') 
with open(schema_filename) as jsonschema:
    neoschema = json.loads(jsonschema.read())

js_files = "modernizr.min.js jquery{min:s}.js bootstrap{min:s}.js ie10-viewport-bug-workaround.js jquery.bootpag.min.js nsweb.js"
js_files = js_files.format(min="" if app.debug else ".min").split()
js_files = [os.path.join("js", f) for f in js_files]
css_files = "bootstrap.readable.min.css nsweb.css".split()
css_files = [os.path.join("css", f) for f in css_files]
js = Bundle(*js_files, filters='uglifyjs', output='js/min.js')
css = Bundle(*css_files, output='css/min.css')
assets.register('js_all', js)
assets.register('css_all', css)

class InputError(Exception):
    pass

@app.route("/")
def route():
    return render_template('sample.html', debug=app.debug, version=version)

@app.route("/Search/", methods=['POST'])
def network():
    try:
        query = request.get_json()
        validate(query, neoschema)
        engine = nsweb.NeoEngine()
        result = engine.search(query)
    except InputError as e:
        return jsonify(errors=[unicode(e)])
    except RuntimeError as e:
        return jsonify(errors=[unicode(e)])
    except ValidationError as e:
        return jsonify(errors=[u"Input error in '{:s}': {:s}.".format(e.path[0], e.message)])

    response = make_response(result)
    response.mimetype = 'application/json'
    return response

if __name__ == "__main__":
    app.run(host='0.0.0.0')

