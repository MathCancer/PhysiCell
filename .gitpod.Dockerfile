FROM gitpod/workspace-full

RUN sudo install-packages inkscape

RUN pip install --upgrade pip && \
    pip install seaborn ipywidgets