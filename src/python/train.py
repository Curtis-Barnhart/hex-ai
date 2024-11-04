import sys

import keras

from data_to_TFRecord import read_from_tfrecords


def generate_model():
    print("Generating new model")
    model = keras.models.Sequential([
        keras.layers.Input((363,)),
        keras.layers.Dense(800, activation='relu'),
        keras.layers.Dense(800, activation='relu'),
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
    dataset = read_from_tfrecords(sys.argv[1:])
    infinite_dataset = dataset.repeat()

    # Find size of dataset so we know how big to make steps_per_epoch
    dataset_size = sum(1 for _ in dataset)
    print("Loaded in {:d} records.".format(dataset_size))

    model.fit(infinite_dataset, steps_per_epoch=dataset_size, epochs=3)

