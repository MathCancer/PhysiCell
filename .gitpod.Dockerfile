FROM gitpod/workspace-full

RUN sudo install-packages ghostscript

RUN pip install --upgrade pip && \
    pip install seaborn ipywidgets