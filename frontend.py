from flask import Flask, render_template, request, Response
app = Flask(__name__)

variants = ["original", "biconf", "yanetal", "option3", "option4", "option7", "option8"]
compressions = [0.1, 0.2, 0.4, 0.5]
keySizes = [128, 256, 512, 1024, 2048, 4096, 8192, 10000,100000,1000000,10000000]
variant="original"
compression=0.1
pa_blocks=10
keySize=1024
qberr=0.01
console_output = []

def print_to_console(message):
    console_output.append(message)
@app.route('/')
def home():
    return render_template('index.html', variants=variants, compressions=compressions, keySizes=keySizes)

@app.route('/set_variant', methods=['POST'])
def set_variant():
    data = request.get_json()
    choice = data['choice']
    variant = choice
    print_to_console(variant)
    return "ok"
@app.route('/set_compression', methods=['POST'])
def set_compression():
    data = request.get_json()
    choice = data['choice']
    compression = float(choice)
    print_to_console(compression)
    return "ok"
@app.route('/set_qber', methods=['POST'])
def set_qber():
    data = request.get_json()
    choice = data['choice']
    qber = float(choice)
    print_to_console(qber)
    return "ok"

@app.route('/set_blocks', methods=['POST'])
def set_blocks():
    data = request.get_json()
    choice = data['choice']
    pa_blocks = int(choice)
    print_to_console(pa_blocks)
    return "ok"
@app.route('/set_keysize', methods=['POST'])
def set_keysize():
    data = request.get_json()
    choice = data['choice']
    keySize = int(choice)
    print_to_console(keySize)
    return "ok"

@app.route('/start_simulation', methods=['POST'])
def start_simulation():
    return "ok"

@app.route('/get_variable')
def get_variable():
    global my_variable
    return str(my_variable)


@app.route('/stream_console')
def stream_console():
    def event_stream():
        while True:
            if console_output:
                yield 'data: {}\n\n'.format(console_output.pop(0))

    return Response(event_stream(), mimetype='text/event-stream')


if __name__ == '__main__':
    app.run(debug=True)


