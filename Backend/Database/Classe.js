const mongoose = require('mongoose')

const classeSchema = new mongoose.Schema({
    nom: {
        type: String,
        required: true,
        unique: true,
        minLength: 3,
        uppercase: true,
    },
    salle: {
        type: String,
        uppercase: true,
    },
    
})

module.exports = mongoose.model('Classe', classeSchema)
