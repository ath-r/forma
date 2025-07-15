![](assets/images/logobg.svg)

**VERONIKA** is a digital emulation of a transistor combo organ manufactured in the GDR during the 1980s. It currently focuses on a specific single-manual model, though voicings from other models may be added in the future.

Implemented features:

* Oscillators generate antialiased square waves using bandlimited steps based on a closed-form approximation of the Sinc integral using trigonometric functions.

* Oscillators use SIMD to compute multiple stops in parallel, e.g. 16′, 8′, 4′, and 2′ stops are computed at the cost of a single oscillator.

* Polysynth-style voice allocation, with a maximum of 16 voices. Oscillators are synced such that identical frequencies don't cancel each other out.

* Accurate *FLUTES II* frequency response voicing from the **Formation 1** model. Mostly accurate, actually. I voiced 5⅓′and 1⅗′ stops to be a little bit duller, so they would work better with the rest of stops. Also, their frequency ratio with fundamental is natural instead of tempered like in the original organ. Tempered mutations are just ugly. Polysynth architecture makes arbitrary tuning such as this very easy to implement.

Planned features (high to low priority):

* Possible *FLUTES II* nonlinearity.

* Non-simultaneous key-on behavior across stops, like in physical organ.

* *SOLO* stops from the **Formation 1**: *Clarinet, Oboe, and Nasal*. Including the non-linear behaviour of formant filters.

* Voicings from the **Formation 2** model.

* Voicings from the **ET 6-1** model.

* Percussion section.

* Phaser section.

* Spring Reverb.
