# Copyright 2024 Curtis Barnhart (cbarnhart@westmont.edu)
# This file is part of hex-ai.
# SPDX-License-Identifier: GPL-3.0-or-later

import sys

import keras

from data_to_TFRecord import read_from_tfrecords


def generate_model():
    print("Generating new model")
    model = keras.models.Sequential([
        keras.layers.Input((363,)),
        keras.layers.Dense(800, activation='relu'),
        keras.layers.Dense(800, activation='relu'),
        keras.layers.Dense(800, activation='relu'),
        keras.layers.Dense(800, activation='relu'),
        keras.layers.Dense(800, activation='relu'),
        keras.layers.Dense(800, activation='relu'),
        keras.layers.Dense(800, activation='relu'),
        keras.layers.Dense(800, activation='relu'),
        keras.layers.Dense(800, activation='relu'),
        # keras.layers.Dropout(0.1),
        keras.layers.Dense(2)
    ])

    model.compile(
        optimizer="adam", 
        loss=keras.losses.BinaryCrossentropy(from_logits=True),
        metrics=["accuracy"]
    )

    return model


if __name__ == "__main__":
    model = generate_model()
    dataset = read_from_tfrecords(sys.argv[1:-2])
    validation = read_from_tfrecords(sys.argv[-2])
    save_model = sys.argv[-1]
    infinite_dataset = dataset.shuffle(10000).repeat()
    infinite_validation = validation.shuffle(10000).repeat()

    # Find size of dataset so we know how big to make steps_per_epoch
    dataset_size = sum(1 for _ in dataset)
    print("Loaded in {:d} training records.".format(dataset_size))
    validation_size = sum(1 for _ in validation)
    print("Loaded in {:d} validation records.".format(validation_size))

    batch_size: int = 2048
    steps: int = dataset_size // batch_size
    validation_steps: int = validation_size // batch_size

    model.fit(
        infinite_dataset,
        batch_size=batch_size,
        steps_per_epoch=steps,
        validation_data=infinite_validation,
        validation_batch_size=batch_size,
        validation_steps=validation_steps,
        epochs=5000,
        callbacks=[
            keras.callbacks.ModelCheckpoint(
                filepath=save_model,
                save_best_only=True,
                verbose=True
            ),
        ]
    )

