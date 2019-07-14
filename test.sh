echo "##################################################" \
&& mypy -p path_search \
&& python3 -m unittest discover \
&& pep8 path_search/ --max-line-length 100 \
&& python3 4x_game_example.py
