export const textToBlob = async () => {
    let data = '03035D2564016F019500000000000000000000000000000000000000000000000000000000000000000000000000000012000300910001010101010101010101010101010101010101010101180FEF01FFFFFFFF020082849710BE4B9B46DE561C44DE8A9D4F7E7F1FD05E5DB7D81E6B3F145E5A3F96BE43C3809E664694DE60C9AA1E664F549E6315EC3F4D3501DF534C165F7F4C129F8F4D107F6F9E575D6F21D51D46A546385D2681D8612718584A275CB852AB1C3858AC9ED85D241BF94EA905B656AA44B7528E04DE380E875E6A0D173D768F97183536EF01FFFFFFFF020082718F10977110E5F74A50403F9AB90F5F90CB903F0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000EC501208CAF012090AF0120240000000D60EF01FFFFFFFF02008203035D1900012001940000000000000000000000000000000000000000000000000000000000000000000000000000000B000200770000000C3303CCFFFFFFFFFEFBBFAEEAAAAAA99995995955554555111444400000000000000000000000000000000000000000000000000000000000000000000000000000000000000000119EEF01FFFFFFFF0200822C08445E560A109E270EC4DE5C904F9E519290';
    let buff = Buffer.from(data)
    let base64data = buff.toString('base64');
    console.log('BASE64DATA', base64data);

    const base64 = await fetch(base64data);
    const base64Response = await fetch(`data:image/jpeg;base64,${base64data}`);
    console.log('RESPONSE', base64);
    console.log('BASE64RESPONSE', base64Response);
    const myblob = await base64.blob();

    console.log('BLOB', myblob);
    let url = URL.createObjectURL(myblob);

    return url;
}

export const textToBlob2 = () => {
    let data = '03035D2564016F019500000000000000000000000000000000000000000000000000000000000000000000000000000012000300910001010101010101010101010101010101010101010101180FEF01FFFFFFFF020082849710BE4B9B46DE561C44DE8A9D4F7E7F1FD05E5DB7D81E6B3F145E5A3F96BE43C3809E664694DE60C9AA1E664F549E6315EC3F4D3501DF534C165F7F4C129F8F4D107F6F9E575D6F21D51D46A546385D2681D8612718584A275CB852AB1C3858AC9ED85D241BF94EA905B656AA44B7528E04DE380E875E6A0D173D768F97183536EF01FFFFFFFF020082718F10977110E5F74A50403F9AB90F5F90CB903F0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000EC501208CAF012090AF0120240000000D60EF01FFFFFFFF02008203035D1900012001940000000000000000000000000000000000000000000000000000000000000000000000000000000B000200770000000C3303CCFFFFFFFFFEFBBFAEEAAAAAA99995995955554555111444400000000000000000000000000000000000000000000000000000000000000000000000000000000000000000119EEF01FFFFFFFF0200822C08445E560A109E270EC4DE5C904F9E519290';
    let ab = new ArrayBuffer(data.length); //bytes is the array with the integer
    let ia = new Uint8Array(ab);

    for (var i = 0; i < data.length; i++) {
    ia[i] = data[i];
    }
console.log(ia);
    // const blob = new Blob([ia], {type: "application/octet-stream"});
    const blob = new Blob([ia], {type: "image/jpeg"});
    console.log('__BLOB__', blob)
    let url = URL.createObjectURL(blob);

    return url;
}

function hexToInt(hexString){

}