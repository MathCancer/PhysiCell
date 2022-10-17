FROM gitpod/workspace-full

RUN sudo install-packages inkscape

FROM python:3

RUN pip install --upgrade pip && \
    pip install seaborn ipywidgets