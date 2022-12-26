const mongoose = require('mongoose')
const moment = require('moment')
const Planning = require('./Planning')
const {toMinutes} = require('./functions')

const presenceSchema = new mongoose.Schema({

    etudiant: {
        type: mongoose.SchemaTypes.ObjectId,
        required: true,
        ref: 'Etudiant'
    },

    dateheure: {
        type: Date,
        required: true,
        default: () => Date.now()
    }
    
})

presenceSchema.virtual('date').get(function(){
    return moment(this.dateheure).format('DD-MM-YYYY')
})

presenceSchema.virtual('heure').get(function(){
    return moment(this.dateheure).format('HH:mm')
})

//Retourne l'objet cours correspondant à l'instant de la présence
presenceSchema.methods.getCours = async function(){

    let jour_sem = moment(this.dateheure).isoWeekday() //Numéro du jour de la semaine
    let jour_min = toMinutes(this.dateheure)

    let planItems = await Planning.find().where('jourSemaine').equals(jour_sem).populate('cours')

    let planItem = planItems.find((value)=>{
        return value.hDebutNumber <= jour_min && value.hFinNumber > jour_min
    })

    if(planItem) return planItem.cours
        
    return undefined
}

module.exports = mongoose.model('Presence', presenceSchema)
