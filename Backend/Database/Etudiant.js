const mongoose = require('mongoose')

const etudiantSchema = new mongoose.Schema({
    matricule: {
        type: String,
        required: true,
        unique: true,
        minLength: 3,
        uppercase: true,
    },
    nom: {
        type: String,
        required: true,
        uppercase: true,
    },
    classe: {
        type: mongoose.SchemaTypes.ObjectId,
        required: true,
        ref: 'Classe'
    },
    empreinte: {
        type: String,
        // required: true,
        // unique: true
    },
    imageEmpreinte: {
        type: mongoose.SchemaTypes.ObjectId,
        ref: 'Image'
    },
    voix: {
        type: mongoose.SchemaTypes.ObjectId,
        ref: 'Audio'
    },
    statut: {
        type: Boolean, //true = enrollement validé, false = enrolement en attente
        required: true,
    }
    
}, { 
    timestamps: { createdAt: 'dateCreation', updatedAt: 'dateModification' } 
    })

module.exports = mongoose.model('Etudiant', etudiantSchema)
